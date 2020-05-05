#include <iostream>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

#include "netspeak/Netspeak.hpp"
#include "netspeak/RetrievalStrategy3.hpp"

namespace netspeak {

/* In terms of unit testing, objects that are responsible for global setup and
 * teardown operations are called fixtures. With Boost.Test you can define
 * fixtures as structs/classes with an constructor/destructor.
 *
 * http://www.boost.org/doc/libs/1_40_0/libs/test/doc/html/utf/user-guide/fixture/test-suite-shared.html
 */

static Netspeak<RetrievalStrategy3Tag> netspeak;

struct netspeak_service_fixture {
  netspeak_service_fixture() {
    BOOST_TEST_MESSAGE("netspeak_service_fixture setup");
    const Configurations::Map config = {
      { Configurations::path_to_home, test::INDEX_DIR },
      { Configurations::cache_capacity, "1000" }
    };
    try {
      netspeak.initialize(config);
    } catch (std::exception& error) {
      std::cerr << "Some error occured: " << error.what() << std::endl;
    }
  }

  ~netspeak_service_fixture() {
    BOOST_TEST_MESSAGE("netspeak_service_fixture teardown");
  }
};

BOOST_GLOBAL_FIXTURE(netspeak_service_fixture);

BOOST_AUTO_TEST_SUITE(netspeak_service)

BOOST_AUTO_TEST_CASE(test_search_with_valid_queries) {
  const std::vector<std::string> test_cases = {
    "* ? {the world}",
    "+ {the world}",
    "[but besides]*",
    "* what *",
    "i love",
    "foo",
    "jr.",
    "foo | bar"
    // Add more valid queries here ...
  };

  for (const auto& query : test_cases) {
    generated::Request request;
    request.set_query(query);

    std::shared_ptr<generated::Response> response = netspeak.search(request);
    // check the encapsulated request
    BOOST_REQUIRE_EQUAL(response->request().query(), query);
    BOOST_REQUIRE_EQUAL(response->request().max_phrase_count(), 100);
    BOOST_REQUIRE_EQUAL(response->request().max_phrase_frequency(),
                        std::numeric_limits<uint64_t>::max());
    BOOST_REQUIRE_EQUAL(response->request().phrase_length_min(), 1);
    BOOST_REQUIRE_EQUAL(response->request().phrase_length_max(), 5);
    BOOST_REQUIRE_EQUAL(response->request().pruning_low(), 130000);
    BOOST_REQUIRE_EQUAL(response->request().pruning_high(), 160000);
    BOOST_REQUIRE_EQUAL(response->request().quantile_low(), 0.5);
    BOOST_REQUIRE_EQUAL(response->request().quantile_high(), 0.8);
    BOOST_REQUIRE_EQUAL(response->request().max_regexword_matches(), 10);
    // check response member for wildcard and non-wildcard queries
    BOOST_REQUIRE_GT(response->query_token_size(), 0);
    BOOST_REQUIRE_GT(response->query().unit_size(), 0);
    BOOST_REQUIRE_GT(response->phrase_size(), 0);
    BOOST_REQUIRE_LE(response->phrase_size(), 100);
    BOOST_REQUIRE_GT(response->total_frequency(), 0);
    BOOST_REQUIRE_EQUAL(response->error_code(),
                        to_ordinal(error_code::no_error));
    BOOST_REQUIRE(response->error_message().empty());
  }
}

BOOST_AUTO_TEST_CASE(test_search_with_invalid_queries) {
  const std::vector<std::string> test_cases = {
    "this is invalid [",      "this is invalid ]",
    "this is invalid {",      "this is invalid }",
    "this is invalid #",      "this is invalid [ # ]",
    "this is invalid [ ? ]",  "this is invalid [ * ]",
    "this is invalid { # }",  "this is invalid { ? }",
    "this is invalid { * }",  "this is invalid { [ ] }",
    "this is invalid [ { } ]"
    // Add more invalid queries here ...
  };

  netspeak::generated::Request request;
  for (const auto& query : test_cases) {
    request.set_query(query);
    std::shared_ptr<generated::Response> response = netspeak.search(request);
    BOOST_REQUIRE_EQUAL(response->error_code(),
                        to_ordinal(error_code::invalid_query));
    BOOST_REQUIRE(!(response->error_message().empty()));
  }

  request.set_query("this query is too long now"); // which is indeed no error
  std::shared_ptr<generated::Response> response = netspeak.search(request);
  BOOST_REQUIRE_EQUAL(response->error_code(),
                      to_ordinal(error_code::invalid_query));
  BOOST_REQUIRE(!(response->error_message().empty()));
}

// Note: The list of words in Response::unknown_word is always sorted
// alphabetically and not by the order of occurrence in the query.
// Furthermore the processing of wildcard queries cancels when some unknown
// word occurs, so the set of unknown words is always one per QueryResult.
BOOST_AUTO_TEST_CASE(test_search_with_unknown_word) {
  generated::Request request;
  std::shared_ptr<generated::Response> response;

  request.set_query("_this-word-is-unknown_");
  response = netspeak.search(request);
  BOOST_REQUIRE_EQUAL(response->unknown_word_size(), 1);
  BOOST_REQUIRE_EQUAL(response->unknown_word(0), "_this-word-is-unknown_");
  BOOST_REQUIRE_EQUAL(response->error_code(), to_ordinal(error_code::no_error));
  BOOST_REQUIRE(response->error_message().empty());

  request.set_query("_this-two-gram_ _is-unknown_");
  response = netspeak.search(request);
  BOOST_REQUIRE_EQUAL(response->unknown_word_size(), 2);
  BOOST_REQUIRE_EQUAL(response->unknown_word(0), "_is-unknown_");
  BOOST_REQUIRE_EQUAL(response->unknown_word(1), "_this-two-gram_");
  BOOST_REQUIRE_EQUAL(response->error_code(), to_ordinal(error_code::no_error));
  BOOST_REQUIRE(response->error_message().empty());

  request.set_query("_this-is_ _also-unknown_ ? *");
  response = netspeak.search(request);
  BOOST_REQUIRE_EQUAL(response->unknown_word_size(), 1);
  BOOST_REQUIRE_EQUAL(response->unknown_word(0), "_this-is_");
  BOOST_REQUIRE_EQUAL(response->error_code(), to_ordinal(error_code::no_error));
  BOOST_REQUIRE(response->error_message().empty());

  request.set_query("hello _also-unknown_ ? *");
  response = netspeak.search(request);
  BOOST_REQUIRE_EQUAL(response->unknown_word_size(), 1);
  BOOST_REQUIRE_EQUAL(response->unknown_word(0), "_also-unknown_");
  BOOST_REQUIRE_EQUAL(response->error_code(), to_ordinal(error_code::no_error));
  BOOST_REQUIRE(response->error_message().empty());
}

BOOST_AUTO_TEST_CASE(test_search_with_raw_response_bug) {
  generated::Request request;
  request.set_query("the * house");
  std::shared_ptr<generated::RawResponse> response =
      netspeak.search_raw(request);
  BOOST_REQUIRE_EQUAL(to_string(response->query()), request.query());
  BOOST_REQUIRE_EQUAL(response->query_result_size(), 4);
  const std::vector<std::string> expected_norm_queries = {
    "the house", "the ? house", "the ? ? house", "the ? ? ? house"
  };
  for (int i = 0; i < response->query_result_size(); ++i) {
    BOOST_REQUIRE_EQUAL(to_string(response->query_result(i).query()),
                        expected_norm_queries.at(i));
  }
}

BOOST_AUTO_TEST_CASE(test_properties) {
  const auto properties = netspeak.properties();
  BOOST_REQUIRE(!properties.empty());
  for (auto it = properties.begin(); it != properties.end(); ++it) {
    if (it->first == netspeak::Properties::cache_top_100)
      continue;
    BOOST_REQUIRE(!it->second.empty());
  }
}

// (trenkman)
BOOST_AUTO_TEST_CASE(test_search_with_quoted_phrase_bug) {
  generated::Request request;
  request.set_query("{ the of life }");
  // Results:
  // #    ngram         frequency             id
  // -------------------------------------------
  // 1    the life of    16535151  3298534893993
  // 2    life of the     9469316  3298534940808
  // 3    of the life     3507490  3298534907870
  std::shared_ptr<generated::Response> response(netspeak.search(request));
  BOOST_REQUIRE_EQUAL(response->error_code(), to_ordinal(error_code::no_error));
  BOOST_REQUIRE_EQUAL(response->unknown_word_size(), 0);
  BOOST_REQUIRE_EQUAL(response->phrase_size(), 3);
}

// (trenkman)
BOOST_AUTO_TEST_CASE(test_search_with_optionset_bug) {
  generated::Request request;
  request.set_query("so [ good beautiful ]");
  // Results:
  // #    ngram          frequency             id
  // --------------------------------------------
  // 1    so good          5603814  2199023255776
  // 2    so beautiful     1471556  2199023277133
  std::shared_ptr<generated::Response> response(netspeak.search(request));
  BOOST_REQUIRE_EQUAL(response->error_code(), to_ordinal(error_code::no_error));
  BOOST_REQUIRE_EQUAL(response->unknown_word_size(), 0);
  BOOST_REQUIRE_EQUAL(response->phrase_size(), 2);
}

// (trenkman)
BOOST_AUTO_TEST_CASE(test_search_with_maxfreq_bug) {
  generated::Request request;
  const uint64_t maxfreq = 200e6; // 200M
  request.set_query("the *");
  request.set_max_phrase_frequency(maxfreq);

  // Results without maxfreq:
  // #    ngram               frequency             id
  // -------------------------------------------------
  // 1    the                43760129185  1099511630212
  // 2    the same            436080161  2199023358712
  // 3    the first           387093132  2199023339409
  // 4    the world           286492739  2199023377456
  // 5    the most            261699781  2199023320691
  // 6    the other           246143010  2199023355360
  // 7    the following       240678040  2199023312981
  // 8    the new             209513100  2199023304536
  // 9    the best            189003982  2199023328246
  // 10   the time            182107517  2199023290240
  // ...

  std::shared_ptr<generated::Response> response(netspeak.search(request));
  const uint32_t num_phrases = response->phrase_size();
  BOOST_REQUIRE_GT(num_phrases, 0);
  for (const auto& phrase : response->phrase()) {
    BOOST_REQUIRE_LE(phrase.frequency(), maxfreq);
  }

  request.clear_max_phrase_frequency();
  response = netspeak.search(request);
  BOOST_REQUIRE_GE(response->phrase_size(), num_phrases);

  request.set_max_phrase_frequency(maxfreq);
  response = netspeak.search(request);
  BOOST_REQUIRE_EQUAL(response->phrase_size(), num_phrases);
  for (const auto& phrase : response->phrase()) {
    BOOST_REQUIRE_LE(phrase.frequency(), maxfreq);
  }
}

// (trenkman) Tue Nov  8 17:27:22 CET 2011
BOOST_AUTO_TEST_CASE(test_search_with_phrase_tag_bug) {
  generated::Request request;
  request.set_query("waiting * #response");
  std::shared_ptr<generated::Response> response = netspeak.search(request);
  for (const auto& phrase : response->phrase()) {
    BOOST_REQUIRE_EQUAL(phrase.word(0).tag(), generated::Phrase::Word::WORD);
    for (int i = 1; i < phrase.word_size() - 1; ++i) {
      BOOST_REQUIRE_EQUAL(phrase.word(i).tag(),
                          generated::Phrase::Word::WORD_FOR_ASTERISK);
    }
    BOOST_REQUIRE_EQUAL(phrase.word(phrase.word_size() - 1).tag(),
                        generated::Phrase::Word::WORD_IN_DICTSET);
  }

  // This response comes partly from the cache
  request.set_query("waiting ? * #response");
  response = netspeak.search(request);
  for (const auto& phrase : response->phrase()) {
    BOOST_REQUIRE_EQUAL(phrase.word(0).tag(), generated::Phrase::Word::WORD);
    BOOST_REQUIRE_EQUAL(phrase.word(1).tag(),
                        generated::Phrase::Word::WORD_FOR_QMARK);
    for (int i = 2; i < phrase.word_size() - 1; ++i) {
      BOOST_REQUIRE_EQUAL(phrase.word(i).tag(),
                          generated::Phrase::Word::WORD_FOR_ASTERISK);
    }
    BOOST_REQUIRE_EQUAL(phrase.word(phrase.word_size() - 1).tag(),
                        generated::Phrase::Word::WORD_IN_DICTSET);
  }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace netspeak
