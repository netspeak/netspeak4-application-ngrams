#include <boost/test/unit_test.hpp>

#include "netspeak/phrase_methods.hpp"

namespace netspeak {

BOOST_AUTO_TEST_SUITE(test_phrase_methods)

BOOST_AUTO_TEST_CASE(test_make_unique_id) {
  generated::Phrase phrase;
  phrase.set_id(make_unique_id(phrase.word_size(), 0));
  BOOST_REQUIRE_EQUAL(0, phrase.id());

  phrase.set_id(make_unique_id(phrase.word_size(), 100));
  BOOST_REQUIRE_EQUAL(100, phrase.id());

  phrase.add_word();
  phrase.set_id(make_unique_id(phrase.word_size(), 0));
  BOOST_REQUIRE_GE(phrase.id(), 1000000000000);
  BOOST_REQUIRE_LE(phrase.id(), 2000000000000);

  phrase.add_word();
  phrase.set_id(make_unique_id(phrase.word_size(), 0));
  BOOST_REQUIRE_GE(phrase.id(), 2000000000000);
  BOOST_REQUIRE_LE(phrase.id(), 3000000000000);

  phrase.add_word();
  phrase.set_id(make_unique_id(phrase.word_size(), 0));
  BOOST_REQUIRE_GE(phrase.id(), 3000000000000);
  BOOST_REQUIRE_LE(phrase.id(), 4000000000000);

  phrase.add_word();
  phrase.set_id(make_unique_id(phrase.word_size(), 0));
  BOOST_REQUIRE_GE(phrase.id(), 4000000000000);
  BOOST_REQUIRE_LE(phrase.id(), 5000000000000);

  phrase.add_word();
  phrase.set_id(make_unique_id(phrase.word_size(), 0));
  BOOST_REQUIRE_GE(phrase.id(), 5000000000000);
  BOOST_REQUIRE_LE(phrase.id(), 6000000000000);
}

BOOST_AUTO_TEST_CASE(test_operator_less) {
  generated::Phrase lhs_phrase;
  generated::Phrase rhs_phrase;
  BOOST_REQUIRE(!(lhs_phrase < rhs_phrase));

  lhs_phrase.set_frequency(10);
  BOOST_REQUIRE(!(lhs_phrase < rhs_phrase));

  rhs_phrase.set_frequency(100);
  BOOST_REQUIRE_LT(lhs_phrase, rhs_phrase);
}

BOOST_AUTO_TEST_CASE(test_operator_greater) {
  generated::Phrase lhs_phrase;
  generated::Phrase rhs_phrase;
  BOOST_REQUIRE(!(lhs_phrase > rhs_phrase));

  rhs_phrase.set_frequency(10);
  BOOST_REQUIRE(!(lhs_phrase > rhs_phrase));

  lhs_phrase.set_frequency(100);
  BOOST_REQUIRE_GT(lhs_phrase, rhs_phrase);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace netspeak
