#include "test_normalization.hpp"

#include <memory>

#include "paths.hpp"

#include "netspeak/regex/DefaultRegexIndex.hpp"


using namespace antlr4;
using namespace netspeak;
using namespace netspeak::generated;

BOOST_AUTO_TEST_SUITE(netspeak_normalization)

std::shared_ptr<regex::DefaultRegexIndex> _cached_regex_index;
regex::DefaultRegexIndex* get_regex_index() {
  if (!_cached_regex_index) {
    auto regex_words = test::load_file("../../test/data/test_Regex.txt");
    _cached_regex_index = std::shared_ptr<regex::DefaultRegexIndex>(
        new regex::DefaultRegexIndex(regex_words));
  }
  return _cached_regex_index.get();
}

std::string normalize_spaces(const std::string& input) {
  std::string output;

  bool lastIsSpace = true; // true because trim leading spaces
  for (const auto c : input) {
    switch (c) {
      case '\r':
      case '\n':
      case '\t':
      case '\v':
      case ' ':
        if (!lastIsSpace) {
          output.push_back(' ');
          lastIsSpace = true;
        }
        break;

      default:
        output.push_back(c);
        lastIsSpace = false;
        break;
    }
  }

  // trim trailing space
  if (lastIsSpace && !output.empty()) {
    output.erase(output.end() - 1, output.end());
  }

  return output;
}

void test_proof(std::vector<netspeak::generated::Query> normQuery,
                std::vector<netspeak::generated::Query> expectedQuery) {
  std::vector<std::string> string_NormQuery;
  std::vector<std::string> string_ExpectedQuery;

  for (auto it = normQuery.begin(); it != normQuery.end(); it++) {
    string_NormQuery.push_back(normalize_spaces(it->DebugString()));
  }

  for (auto it = expectedQuery.begin(); it != expectedQuery.end(); it++) {
    string_ExpectedQuery.push_back(normalize_spaces(it->DebugString()));
  }

  std::sort(string_NormQuery.begin(), string_NormQuery.end());
  std::sort(string_ExpectedQuery.begin(), string_ExpectedQuery.end());


  BOOST_CHECK_EQUAL_COLLECTIONS(
      string_NormQuery.begin(), string_NormQuery.end(),
      string_ExpectedQuery.begin(), string_ExpectedQuery.end());
}

#define REQUIRE_EQUAL_NORMALIZE_SPACES(actual, expected) \
  BOOST_REQUIRE_EQUAL(normalize_spaces(actual), normalize_spaces(expected));

#define CHECK_EQUAL_COLLECTIONS_ALL(a, b) \
  BOOST_CHECK_EQUAL_COLLECTIONS((a).begin(), (a).end(), (b).begin(), (b).end());


// normalization test for queries consisting of a word and a questionmark
BOOST_AUTO_TEST_CASE(test_normalize_word_qmark) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test ?");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_qmark());


  std::vector<std::string> tokens{ "test", "?" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: QMARK text: "?" }
  )");
}

// normalization test for queries consisting of a regular expression and a
// questionmark
BOOST_AUTO_TEST_CASE(test_normalize_regexword_qmark) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("t* ?");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, regexword_qmark());


  std::vector<std::string> tokens{ "t*", "?" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: REGEXWORD text: "t*" }
    unit { tag: QMARK text: "?" }
  )");
}

// normalization test for queries consisting of a word and an asterisk
BOOST_AUTO_TEST_CASE(test_normalize_word_asterisk) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test *");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_asterisk());


  std::vector<std::string> tokens{ "test", "*" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: ASTERISK text: "*" }
  )");
}

// normalization test for queries consisting of a regular expression and an
// asterisk
BOOST_AUTO_TEST_CASE(test_normalize_regexword_asterisk) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;
  std::vector<Query> expectedQuery;

  Request request;
  request.set_query("t* *");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, regexword_asterisk());


  std::vector<std::string> tokens{ "t*", "*" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: REGEXWORD text: "t*" }
    unit { tag: ASTERISK text: "*" }
  )");
}

// normalization test for queries consisting of a word and a plus
BOOST_AUTO_TEST_CASE(test_normalize_word_plus) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;
  std::vector<Query> expectedQuery;

  Request request;
  request.set_query("test +");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_plus());


  std::vector<std::string> tokens{ "test", "+" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: PLUS text: "+" }
  )");
}

// normalization test for queries consisting of a regular expression and a plus
BOOST_AUTO_TEST_CASE(test_normalize_regexword_plus) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;
  std::vector<Query> expectedQuery;

  Request request;
  request.set_query("t* +");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, regexword_plus());


  std::vector<std::string> tokens{ "t*", "+" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: REGEXWORD text: "t*" }
    unit { tag: PLUS text: "+" }
  )");
}

// normalization test for queries consisting of a word and an optionset with a
// word
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_word) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [test]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);


  test_proof(normQuery, word_optionset_one_word());


  std::vector<std::string> tokens{ "test", "[", "test", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ test ]" }
  )");
}

// normalization test for queries consisting of a word and an optionset with a
// regular expression
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_regexword) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [t*]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);


  test_proof(normQuery, word_optionset_one_regexword());


  std::vector<std::string> tokens{ "test", "[", "t*", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ t* ]" }
  )");
}

// normalization test for queries consisting of a word and an optionset with two
// words
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_two_words) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [hello test]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_optionset_two_words());


  std::vector<std::string> tokens{ "test", "[", "hello", "test", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ hello test ]" }
  )");
}

// normalization test for queries consisting of a word and an optionset with two
// regular expressions
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_two_regexwords) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [t* t+]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, word_optionset_two_regexwords());


  std::vector<std::string> tokens{ "test", "[", "t*", "t+", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ t* t+ ]" }
  )");
}

// normalization test for queries consisting of a word and an optionset with
// some words bundled in a phrase
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_one_phrase_words) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [\"test hello\"]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_optionset_one_phrase_words());


  std::vector<std::string> tokens{
    "test", "[", "\"", "test", "hello", "\"", "]"
  };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ \" test hello \" ]" }
  )");
}

// normalization test for queries consisting of a word and an optionset with a
// regular expression in a phrase
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_one_phrase_regex) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [\"t* t+\"]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, word_optionset_one_phrase_regexwords());


  std::vector<std::string> tokens{ "test", "[", "\"", "t*", "t+", "\"", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ \" t* t+ \" ]" }
  )");
}

// normalization test for queries consisting of a word, an optionset with some
// words and a word
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_phrase_word) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [\"test hello\" hello]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_optionset_phrase_words());


  std::vector<std::string> tokens{ "test",  "[",  "\"",    "test",
                                   "hello", "\"", "hello", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ \" test hello \" hello ]" }
  )");
}

// normalization test for queries consisting of a word, an optionset with a
// regular expression and a regular expression
BOOST_AUTO_TEST_CASE(test_normalize_word_optionset_phrase_regexword) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test [\"t* hello\" t*]");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, word_optionset_phrase_regexword());


  std::vector<std::string> tokens{ "test",  "[",  "\"", "t*",
                                   "hello", "\"", "t*", "]" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: OPTIONSET text: "[ \" t* hello \" t* ]" }
  )");
}

// normalization test for queries consisting of a word and an orderset with some
// words
BOOST_AUTO_TEST_CASE(test_normalize_word_orderset_words) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test {i am tired}");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_orderset_words());


  std::vector<std::string> tokens{ "test", "{", "i", "am", "tired", "}" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: ORDERSET text: "{ i am tired }" }
  )");
}

// normalization test for queries consisting of a word and an orderset with a
// regular expression and some words
BOOST_AUTO_TEST_CASE(test_normalize_word_orderset_regexwords) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test {i am t*}");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, word_orderset_regexwords());


  std::vector<std::string> tokens{ "test", "{", "i", "am", "t*", "}" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: ORDERSET text: "{ i am t* }" }
  )");
}

// normalization test for queries consisting of a word and an orderset with
// words as a phrase
BOOST_AUTO_TEST_CASE(test_normalize_word_orderset_phrase_word) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;
  Request request;
  request.set_query("test {\"i am\" tired}");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, NULL, &normQuery);

  test_proof(normQuery, word_orderset_phrase_word());

  // TODO: Is this really the correct behavior?

  std::vector<std::string> tokens{ "test", "{",  "\"",    "i",
                                   "am",   "\"", "tired", "}" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: ORDERSET text: "{ \" i am \" tired }" }
  )");
}

// normalization test for queries consisting of a word and an orderset with a
// regular expression and a word as phrase
BOOST_AUTO_TEST_CASE(test_normalize_word_orderset_phrase_regexword) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;
  Request request;
  request.set_query("test {\"i t*\" tired}");
  response->mutable_request()->CopyFrom(request);

  netspeak::parse(response, NULL, get_regex_index(), &normQuery);

  test_proof(normQuery, word_orderset_phrase_regexword());


  std::vector<std::string> tokens{ "test", "{",  "\"",    "i",
                                   "t*",   "\"", "tired", "}" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: ORDERSET text: "{ \" i t* \" tired }" }
  )");
}

// normalization test for queries consisting of a word and a dictset operator
// bundled with a word
BOOST_AUTO_TEST_CASE(test_normalize_word_dictset_word) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test #test");
  response->mutable_request()->CopyFrom(request);

  Dictionaries::Map hash_dictionary_;
  const auto dict = Dictionaries::read_from_file(test::DICTIONARY_SET);
  for (const auto& pair : dict) {
    hash_dictionary_.insert(pair);
  }

  netspeak::parse(response, &hash_dictionary_, NULL, &normQuery);
  test_proof(normQuery, word_dictset_word());


  std::vector<std::string> tokens{ "test", "#", "test" };
  CHECK_EQUAL_COLLECTIONS_ALL(tokens, *(response->mutable_query_token()));

  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: DICTSET text: "# test" }
  )");
}

// normalization test for queries consisting of a word and a dictset operator
// bundled with a regular expression expected to fail
BOOST_AUTO_TEST_CASE(test_fail_dictset_regexword) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query("test #t*");
  response->mutable_request()->CopyFrom(request);

  BOOST_CHECK_THROW(netspeak::parse(response, NULL, NULL, &normQuery),
                    netspeak::invalid_query);
}

// test for multiple different queries bundled in one using the "|" operator
BOOST_AUTO_TEST_CASE(test_split_operator) {
  std::shared_ptr<RawResponse> response(new RawResponse());
  std::vector<Query> normQuery;

  Request request;
  request.set_query(
      "test ? | t* * | test + | test [test] | "
      "test [\"test hello\"] | test {i am tired}"
      "| test #test");
  response->mutable_request()->CopyFrom(request);

  Dictionaries::Map hash_dictionary_;
  const auto dict = Dictionaries::read_from_file(test::DICTIONARY_SET);
  for (const auto& pair : dict) {
    hash_dictionary_.insert(pair);
  }

  netspeak::parse(response, &hash_dictionary_, get_regex_index(), &normQuery);

  std::vector<Query> expectedQuery;
  auto add_expected = [&](const std::vector<Query>& queries) {
    expectedQuery.insert(expectedQuery.end(), queries.begin(), queries.end());
  };
  add_expected(word_qmark());
  add_expected(regexword_asterisk());
  add_expected(word_plus());
  add_expected(word_optionset_one_word());
  add_expected(word_optionset_one_phrase_words());
  add_expected(word_orderset_words());
  add_expected(word_dictset_word());

  test_proof(normQuery, expectedQuery);

  // TODO: Fix bug
  REQUIRE_EQUAL_NORMALIZE_SPACES(response->query().DebugString(),
                                 R"(
    unit { tag: WORD text: "test" }
    unit { tag: QMARK text: "?" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASTERISK text: "*" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: PLUS text: "+" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: OPTIONSET text: "t* test test [ test ]" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: OPTIONSET text: "test [ \" test hello \" ]" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ORDERSET text: "test { i am tired }" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: ASSOCIATION text: "|" }
    unit { tag: DICTSET text: "test # test" }
  )");
}


BOOST_AUTO_TEST_SUITE_END()

using namespace generated;

static std::vector<std::string> regexwordslist{ "test", "tester", "total",
                                                "toronto" };
static std::vector<std::string> dictsetList{ "test", "screen", "examination",
                                             "prove" };


const std::vector<Query> word_qmark() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  Query::Unit qmark;
  qmark.set_text(std::string(1, wildcard::qmark));
  qmark.set_tag(Query::Unit::QMARK);

  query.add_unit()->CopyFrom(word);
  query.add_unit()->CopyFrom(qmark);


  vector.push_back(query);

  return vector;
}

const std::vector<Query> regexword_qmark() {
  std::vector<Query> vector;

  Query::Unit regexword;
  regexword.set_tag(Query::Unit::WORD_FOR_REGEX);

  Query::Unit qmark;
  qmark.set_text(std::string(1, wildcard::qmark));
  qmark.set_tag(Query::Unit::QMARK);

  for (std::vector<std::string>::iterator word = regexwordslist.begin();
       word != regexwordslist.end(); word++) {
    regexword.set_text(*word);
    Query query;
    query.add_unit()->CopyFrom(regexword);
    query.add_unit()->CopyFrom(qmark);
    vector.push_back(query);
  }

  return vector;
}


const std::vector<Query> word_asterisk() {
  std::vector<Query> vector;


  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  Query::Unit asterisk;
  asterisk.set_text(std::string(1, wildcard::qmark));
  asterisk.set_tag(Query::Unit::QMARK_FOR_ASTERISK);

  for (int i = 1; i <= 5; i++) {
    Query query;
    query.add_unit()->CopyFrom(word);
    for (int j = 5; j > i; j--) {
      query.add_unit()->CopyFrom(asterisk);
    }
    vector.push_back(query);
  }

  return vector;
}


const std::vector<Query> regexword_asterisk() {
  std::vector<Query> vector;

  Query::Unit regexword;
  regexword.set_tag(Query::Unit::WORD_FOR_REGEX);

  Query::Unit asterisk;
  asterisk.set_text(std::string(1, wildcard::qmark));
  asterisk.set_tag(Query::Unit::QMARK_FOR_ASTERISK);

  for (std::vector<std::string>::iterator word = regexwordslist.begin();
       word != regexwordslist.end(); word++) {
    regexword.set_text(*word);
    Query query;
    query.add_unit()->CopyFrom(regexword);
    for (int i = 1; i <= 5; i++) {
      Query temp(query);
      for (int j = 5; j > i; j--) {
        temp.add_unit()->CopyFrom(asterisk);
      }
      vector.push_back(temp);
    }
  }
  return vector;
}

const std::vector<Query> word_plus() {
  std::vector<Query> vector;


  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  Query::Unit plus;
  plus.set_text(std::string(1, wildcard::qmark));
  plus.set_tag(Query::Unit::QMARK_FOR_PLUS);

  for (int i = 1; i < 5; i++) {
    Query query;
    query.add_unit()->CopyFrom(word);
    for (int j = 5; j > i; j--) {
      query.add_unit()->CopyFrom(plus);
    }
    vector.push_back(query);
  }

  return vector;
}

const std::vector<Query> regexword_plus() {
  std::vector<Query> vector;

  Query::Unit regexword;
  regexword.set_tag(Query::Unit::WORD_FOR_REGEX);

  Query::Unit plus;
  plus.set_text(std::string(1, wildcard::qmark));
  plus.set_tag(Query::Unit::QMARK_FOR_PLUS);

  for (std::vector<std::string>::iterator word = regexwordslist.begin();
       word != regexwordslist.end(); word++) {
    regexword.set_text(*word);
    Query query;
    query.add_unit()->CopyFrom(regexword);
    for (int i = 1; i < 5; i++) {
      Query temp(query);
      for (int j = 5; j > i; j--) {
        temp.add_unit()->CopyFrom(plus);
      }
      vector.push_back(temp);
    }
  }
  return vector;
}


const std::vector<Query> word_optionset_one_word() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  query.add_unit()->CopyFrom(word);
  vector.push_back(query);

  Query query2(query);

  Query::Unit optionset;
  optionset.set_text("test");
  optionset.set_tag(Query::Unit::WORD_IN_OPTIONSET);


  query2.add_unit()->CopyFrom(optionset);

  vector.push_back(query2);
  return vector;
}

const std::vector<Query> word_optionset_one_regexword() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  query.add_unit()->CopyFrom(word);
  vector.push_back(query);

  Query::Unit optionset;

  optionset.set_tag(Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET);

  for (auto it = regexwordslist.begin(); it != regexwordslist.end(); it++) {
    Query query2(query);
    optionset.set_text(*it);
    query2.add_unit()->CopyFrom(optionset);
    vector.push_back(query2);
  }

  return vector;
}


const std::vector<Query> word_optionset_two_words() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  Query::Unit optionset1;
  optionset1.set_text("test");
  optionset1.set_tag(Query::Unit::WORD_IN_OPTIONSET);

  query.add_unit()->CopyFrom(word);
  query.add_unit()->CopyFrom(optionset1);
  vector.push_back(query);

  Query query2;

  Query::Unit optionset2;
  optionset2.set_text("hello");
  optionset2.set_tag(Query::Unit::WORD_IN_OPTIONSET);

  query2.add_unit()->CopyFrom(word);
  query2.add_unit()->CopyFrom(optionset2);
  vector.push_back(query2);

  return vector;
}

const std::vector<Query> word_optionset_two_regexwords() {
  std::vector<Query> vector;


  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  Query::Unit optionset1;
  optionset1.set_tag(Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET);

  for (auto it = regexwordslist.begin(); it != regexwordslist.end(); it++) {
    Query query;
    optionset1.set_text(*it);
    query.add_unit()->CopyFrom(word);
    query.add_unit()->CopyFrom(optionset1);
    vector.push_back(query);
    vector.push_back(query);
  }

  return vector;
}


const std::vector<Query> word_optionset_one_phrase_words() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  query.add_unit()->CopyFrom(word);
  vector.push_back(query);

  Query query2(query);

  Query::Unit optionset;
  optionset.set_text("test");
  optionset.set_tag(Query::Unit::WORD_IN_OPTIONSET);

  Query::Unit optionset2;
  optionset2.set_text("hello");
  optionset2.set_tag(Query::Unit::WORD_IN_OPTIONSET);

  query2.add_unit()->CopyFrom(optionset);
  query2.add_unit()->CopyFrom(optionset2);

  vector.push_back(query2);
  return vector;
}

const std::vector<Query> word_optionset_one_phrase_regexwords() {
  std::vector<Query> vector;
  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);

  Query query;
  query.add_unit()->CopyFrom(word);
  vector.push_back(query);

  Query::Unit optionset;
  optionset.set_tag(Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET);

  Query::Unit optionset2;
  ;
  optionset2.set_tag(Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET);

  for (auto it = regexwordslist.begin(); it != regexwordslist.end(); it++) {
    for (auto it2 = regexwordslist.begin(); it2 != regexwordslist.end();
         it2++) {
      Query query2(query);
      optionset.set_text(*it);
      query2.add_unit()->CopyFrom(optionset);

      optionset2.set_text(*it2);
      query2.add_unit()->CopyFrom(optionset2);
      vector.push_back(query2);
    }
  }
  return vector;
}

const std::vector<Query> word_optionset_phrase_words() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  Query::Unit optionset1;
  optionset1.set_text("test");
  optionset1.set_tag(Query::Unit::WORD_IN_OPTIONSET);
  Query::Unit optionset2;
  optionset2.set_text("hello");
  optionset2.set_tag(Query::Unit::WORD_IN_OPTIONSET);

  query.add_unit()->CopyFrom(word);
  Query query2(query);

  query.add_unit()->CopyFrom(optionset1);
  query.add_unit()->CopyFrom(optionset2);
  vector.push_back(query);

  query2.add_unit()->CopyFrom(optionset2);
  vector.push_back(query2);

  return vector;
}


const std::vector<Query> word_optionset_phrase_regexword() {
  std::vector<Query> vector;


  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  Query::Unit optionset1;
  optionset1.set_tag(Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET);
  Query::Unit optionset2;
  optionset2.set_text("hello");
  optionset2.set_tag(Query::Unit::WORD_IN_OPTIONSET);


  for (auto it = regexwordslist.begin(); it != regexwordslist.end(); it++) {
    Query query;
    Query query2;

    optionset1.set_text(*it);
    query.add_unit()->CopyFrom(word);
    query2.add_unit()->CopyFrom(word);

    query.add_unit()->CopyFrom(optionset1);
    query.add_unit()->CopyFrom(optionset2);
    vector.push_back(query);

    query2.add_unit()->CopyFrom(optionset1);
    vector.push_back(query2);
  }
  return vector;
}


const std::vector<Query> word_orderset_words() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  query.add_unit()->CopyFrom(word);

  Query::Unit orderset1;
  Query::Unit orderset2;
  Query::Unit orderset3;

  orderset1.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset1.set_text("i");

  orderset2.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset2.set_text("am");

  orderset3.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset3.set_text("tired");

  Query query1(query);
  Query query2(query);
  Query query3(query);
  Query query4(query);
  Query query5(query);
  Query query6(query);

  query1.add_unit()->CopyFrom(orderset1);
  query1.add_unit()->CopyFrom(orderset2);
  query1.add_unit()->CopyFrom(orderset3);

  vector.push_back(query1);

  query2.add_unit()->CopyFrom(orderset1);
  query2.add_unit()->CopyFrom(orderset3);
  query2.add_unit()->CopyFrom(orderset2);

  vector.push_back(query2);

  query3.add_unit()->CopyFrom(orderset2);
  query3.add_unit()->CopyFrom(orderset1);
  query3.add_unit()->CopyFrom(orderset3);

  vector.push_back(query3);

  query4.add_unit()->CopyFrom(orderset2);
  query4.add_unit()->CopyFrom(orderset3);
  query4.add_unit()->CopyFrom(orderset1);

  vector.push_back(query4);

  query5.add_unit()->CopyFrom(orderset3);
  query5.add_unit()->CopyFrom(orderset1);
  query5.add_unit()->CopyFrom(orderset2);

  vector.push_back(query5);

  query6.add_unit()->CopyFrom(orderset3);
  query6.add_unit()->CopyFrom(orderset2);
  query6.add_unit()->CopyFrom(orderset1);

  vector.push_back(query6);

  return vector;
}


const std::vector<Query> word_orderset_regexwords() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  query.add_unit()->CopyFrom(word);

  Query::Unit orderset1;
  Query::Unit orderset2;
  Query::Unit orderset3;

  orderset1.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset1.set_text("i");

  orderset2.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset2.set_text("am");

  orderset3.set_tag(Query::Unit::WORD_FOR_REGEX_IN_ORDERSET);


  for (auto it = regexwordslist.begin(); it != regexwordslist.end(); it++) {
    Query query1(query);
    Query query2(query);
    Query query3(query);
    Query query4(query);
    Query query5(query);
    Query query6(query);

    orderset3.set_text(*it);

    query1.add_unit()->CopyFrom(orderset1);
    query1.add_unit()->CopyFrom(orderset2);
    query1.add_unit()->CopyFrom(orderset3);

    vector.push_back(query1);

    query2.add_unit()->CopyFrom(orderset1);
    query2.add_unit()->CopyFrom(orderset3);
    query2.add_unit()->CopyFrom(orderset2);

    vector.push_back(query2);

    query3.add_unit()->CopyFrom(orderset2);
    query3.add_unit()->CopyFrom(orderset1);
    query3.add_unit()->CopyFrom(orderset3);

    vector.push_back(query3);

    query4.add_unit()->CopyFrom(orderset2);
    query4.add_unit()->CopyFrom(orderset3);
    query4.add_unit()->CopyFrom(orderset1);

    vector.push_back(query4);

    query5.add_unit()->CopyFrom(orderset3);
    query5.add_unit()->CopyFrom(orderset1);
    query5.add_unit()->CopyFrom(orderset2);

    vector.push_back(query5);

    query6.add_unit()->CopyFrom(orderset3);
    query6.add_unit()->CopyFrom(orderset2);
    query6.add_unit()->CopyFrom(orderset1);

    vector.push_back(query6);
  }

  return vector;
}

const std::vector<Query> word_orderset_phrase_word() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  query.add_unit()->CopyFrom(word);

  Query::Unit orderset1;
  Query::Unit orderset2;
  Query::Unit orderset3;

  orderset1.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset1.set_text("i");

  orderset2.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset2.set_text("am");

  orderset3.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset3.set_text("tired");

  Query query1(query);
  Query query2(query);

  query1.add_unit()->CopyFrom(orderset1);
  query1.add_unit()->CopyFrom(orderset2);
  query1.add_unit()->CopyFrom(orderset3);

  vector.push_back(query1);

  query2.add_unit()->CopyFrom(orderset3);
  query2.add_unit()->CopyFrom(orderset1);
  query2.add_unit()->CopyFrom(orderset2);

  vector.push_back(query2);

  return vector;
}

const std::vector<Query> word_orderset_phrase_regexword() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  query.add_unit()->CopyFrom(word);

  Query::Unit orderset1;
  Query::Unit orderset2;
  Query::Unit orderset3;

  orderset1.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset1.set_text("i");

  orderset2.set_tag(Query::Unit::WORD_FOR_REGEX_IN_ORDERSET);


  orderset3.set_tag(Query::Unit::WORD_IN_ORDERSET);
  orderset3.set_text("tired");

  for (auto it = regexwordslist.begin(); it != regexwordslist.end(); it++) {
    Query query1(query);
    Query query2(query);

    orderset2.set_text(*it);

    query1.add_unit()->CopyFrom(orderset1);
    query1.add_unit()->CopyFrom(orderset2);
    query1.add_unit()->CopyFrom(orderset3);

    vector.push_back(query1);

    query2.add_unit()->CopyFrom(orderset3);
    query2.add_unit()->CopyFrom(orderset1);
    query2.add_unit()->CopyFrom(orderset2);

    vector.push_back(query2);
  }
  return vector;
}


const std::vector<Query> word_dictset_word() {
  std::vector<Query> vector;
  Query query;

  Query::Unit word;
  word.set_text("test");
  word.set_tag(Query::Unit::WORD);
  query.add_unit()->CopyFrom(word);

  Query::Unit synonym;
  synonym.set_tag(Query::Unit::WORD_IN_DICTSET);


  for (auto word = dictsetList.begin(); word != dictsetList.end(); word++) {
    Query temp(query);
    synonym.set_text(*word);
    temp.add_unit()->CopyFrom(synonym);
    vector.push_back(temp);
  }

  return vector;
}
