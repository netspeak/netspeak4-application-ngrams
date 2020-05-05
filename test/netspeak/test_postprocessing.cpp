#include <boost/test/unit_test.hpp>

#include "netspeak/postprocessing.hpp"

namespace netspeak {

BOOST_AUTO_TEST_SUITE(test_postprocessing)

BOOST_AUTO_TEST_CASE(test_tag_phrase) {
  generated::Query query;
  generated::Phrase phrase;
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));

  generated::Phrase::Word* word = phrase.add_word();
  BOOST_REQUIRE_THROW(tag_phrase(phrase, query), std::runtime_error);
  phrase.clear_word();

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::QMARK));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD_FOR_QMARK, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::QMARK_FOR_ASTERISK));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD_FOR_ASTERISK, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::WORD));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::WORD_IN_DICTSET));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD_IN_DICTSET, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::WORD_IN_DICTSET));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD_IN_DICTSET, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::WORD_IN_OPTIONSET));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD_IN_OPTIONSET, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::WORD_IN_ORDERSET));
  BOOST_REQUIRE_NO_THROW(tag_phrase(phrase, query));
  BOOST_REQUIRE_EQUAL(generated::Phrase::Word::WORD_IN_ORDERSET, word->tag());

  word = phrase.add_word();
  append(query, make_unit(generated::Query::Unit::ASTERISK));
  BOOST_REQUIRE_THROW(tag_phrase(phrase, query), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace netspeak
