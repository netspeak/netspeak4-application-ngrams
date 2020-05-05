#include <vector>

#include <boost/test/unit_test.hpp>

#include "netspeak/query_methods.hpp"

namespace netspeak {

BOOST_AUTO_TEST_SUITE(test_query_methods)

BOOST_AUTO_TEST_CASE(test_append) {
  generated::Query query;
  auto unit = make_unit(generated::Query_Unit::WORD, "WORD");
  append(query, unit);
  BOOST_REQUIRE_EQUAL(unit, *std::prev(query.unit().end()));

  unit = make_unit(generated::Query_Unit::ASTERISK);
  append(query, unit);
  BOOST_REQUIRE_EQUAL(unit, *std::prev(query.unit().end()));
}

BOOST_AUTO_TEST_CASE(test_append_iter) {
  const std::vector<generated::Query::Unit> expected_units = {
    make_unit(generated::Query_Unit::ASTERISK),
    make_unit(generated::Query_Unit::QMARK),
    make_unit(generated::Query_Unit::QMARK_FOR_ASTERISK),
    make_unit(generated::Query_Unit::DICTSET, "DICTSET"),
    make_unit(generated::Query_Unit::OPTIONSET, "OPTIONSET"),
    make_unit(generated::Query_Unit::ORDERSET, "ORDERSET"),
    make_unit(generated::Query_Unit::WORD, "WORD"),
    make_unit(generated::Query_Unit::WORD_IN_DICTSET, "WORD_IN_DICTSET"),
    make_unit(generated::Query_Unit::WORD_IN_OPTIONSET, "WORD_IN_OPTIONSET"),
    make_unit(generated::Query_Unit::WORD_IN_ORDERSET, "WORD_IN_ORDERSET")
  };

  generated::Query query;
  append(query, expected_units.begin(), expected_units.end());

  for (unsigned i = 0; i != expected_units.size(); ++i) {
    BOOST_REQUIRE_EQUAL(expected_units.at(i), query.unit(i));
  }
}

BOOST_AUTO_TEST_CASE(test_count_unit) {
  const std::vector<generated::Query::Unit> expected_units = {
    make_unit(generated::Query_Unit::ASTERISK),
    make_unit(generated::Query_Unit::ASTERISK),
    make_unit(generated::Query_Unit::DICTSET, "DICTSET"),
    make_unit(generated::Query_Unit::OPTIONSET, "OPTIONSET"),
    make_unit(generated::Query_Unit::ORDERSET, "ORDERSET"),
    make_unit(generated::Query_Unit::WORD, "WORD"),
    make_unit(generated::Query_Unit::WORD, "WORD")
  };

  generated::Query query;
  append(query, expected_units.begin(), expected_units.end());

  BOOST_REQUIRE_EQUAL(2, count_unit(query, generated::Query_Unit::ASTERISK));
  BOOST_REQUIRE_EQUAL(1, count_unit(query, generated::Query_Unit::DICTSET));
  BOOST_REQUIRE_EQUAL(1, count_unit(query, generated::Query_Unit::OPTIONSET));
  BOOST_REQUIRE_EQUAL(1, count_unit(query, generated::Query_Unit::ORDERSET));
  BOOST_REQUIRE_EQUAL(2, count_unit(query, generated::Query_Unit::WORD));
}

BOOST_AUTO_TEST_CASE(test_has_unit) {
  const std::vector<generated::Query::Unit> expected_units = {
    make_unit(generated::Query_Unit::ASTERISK),
    make_unit(generated::Query_Unit::OPTIONSET, "OPTIONSET"),
    make_unit(generated::Query_Unit::ORDERSET, "ORDERSET")
  };

  generated::Query query;
  append(query, expected_units.begin(), expected_units.end());

  BOOST_REQUIRE(has_unit(query, generated::Query_Unit::ASTERISK));
  BOOST_REQUIRE(has_unit(query, generated::Query_Unit::OPTIONSET));
  BOOST_REQUIRE(has_unit(query, generated::Query_Unit::ORDERSET));
  BOOST_REQUIRE(!has_unit(query, generated::Query_Unit::DICTSET));
  BOOST_REQUIRE(!has_unit(query, generated::Query_Unit::WORD));
}

BOOST_AUTO_TEST_CASE(test_has_wildcard) {
  generated::Query query;
  append(query, make_unit(generated::Query_Unit::WORD, "WORD"));
  append(query, make_unit(generated::Query_Unit::WORD_IN_OPTIONSET,
                          "WORD_IN_OPTIONSET"));
  append(query, make_unit(generated::Query_Unit::WORD_IN_ORDERSET,
                          "WORD_IN_ORDERSET"));
  append(query,
         make_unit(generated::Query_Unit::WORD_IN_DICTSET, "WORD_IN_DICTSET"));
  BOOST_REQUIRE(!has_wildcard(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::ASTERISK));
  BOOST_REQUIRE(has_wildcard(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::QMARK));
  BOOST_REQUIRE(has_wildcard(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::QMARK_FOR_ASTERISK));
  BOOST_REQUIRE(has_wildcard(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::DICTSET, "DICTSET"));
  BOOST_REQUIRE(has_wildcard(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::OPTIONSET, "OPTIONSET"));
  BOOST_REQUIRE(has_wildcard(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::ORDERSET, "ORDERSET"));
  BOOST_REQUIRE(has_wildcard(query));
}

BOOST_AUTO_TEST_CASE(test_is_normalized) {
  generated::Query query;
  append(query, make_unit(generated::Query_Unit::QMARK));
  append(query, make_unit(generated::Query_Unit::QMARK_FOR_ASTERISK));
  append(query, make_unit(generated::Query_Unit::WORD, "WORD"));
  append(query,
         make_unit(generated::Query_Unit::WORD_IN_DICTSET, "WORD_IN_DICTSET"));
  append(query, make_unit(generated::Query_Unit::WORD_IN_OPTIONSET,
                          "WORD_IN_OPTIONSET"));
  append(query, make_unit(generated::Query_Unit::WORD_IN_ORDERSET,
                          "WORD_IN_ORDERSET"));
  BOOST_REQUIRE(is_normalized(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::ASTERISK));
  BOOST_REQUIRE(!is_normalized(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::DICTSET, "DICTSET"));
  BOOST_REQUIRE(!is_normalized(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::OPTIONSET, "OPTIONSET"));
  BOOST_REQUIRE(!is_normalized(query));

  query.Clear();
  append(query, make_unit(generated::Query_Unit::ORDERSET, "ORDERSET"));
  BOOST_REQUIRE(!is_normalized(query));
}

BOOST_AUTO_TEST_CASE(test_make_unit) {
  generated::Query::Unit unit;

  unit = make_unit(generated::Query_Unit::ASTERISK, "ignored");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::ASTERISK, unit.tag());
  BOOST_REQUIRE_EQUAL("*", unit.text());

  unit = make_unit(generated::Query_Unit::QMARK, "ignored");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::QMARK, unit.tag());
  BOOST_REQUIRE_EQUAL("?", unit.text());

  unit = make_unit(generated::Query_Unit::QMARK_FOR_ASTERISK, "ignored");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::QMARK_FOR_ASTERISK, unit.tag());
  BOOST_REQUIRE_EQUAL("?", unit.text());

  unit = make_unit(generated::Query_Unit::DICTSET, "DICTSET");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::DICTSET, unit.tag());
  BOOST_REQUIRE_EQUAL("DICTSET", unit.text());

  unit = make_unit(generated::Query_Unit::OPTIONSET, "OPTIONSET");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::OPTIONSET, unit.tag());
  BOOST_REQUIRE_EQUAL("OPTIONSET", unit.text());

  unit = make_unit(generated::Query_Unit::ORDERSET, "ORDERSET");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::ORDERSET, unit.tag());
  BOOST_REQUIRE_EQUAL("ORDERSET", unit.text());

  unit = make_unit(generated::Query_Unit::WORD, "WORD");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::WORD, unit.tag());
  BOOST_REQUIRE_EQUAL("WORD", unit.text());

  unit = make_unit(generated::Query_Unit::WORD_IN_DICTSET, "WORD_IN_DICTSET");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::WORD_IN_DICTSET, unit.tag());
  BOOST_REQUIRE_EQUAL("WORD_IN_DICTSET", unit.text());

  unit =
      make_unit(generated::Query_Unit::WORD_IN_OPTIONSET, "WORD_IN_OPTIONSET");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::WORD_IN_OPTIONSET, unit.tag());
  BOOST_REQUIRE_EQUAL("WORD_IN_OPTIONSET", unit.text());

  unit = make_unit(generated::Query_Unit::WORD_IN_ORDERSET, "WORD_IN_ORDERSET");
  BOOST_REQUIRE_EQUAL(generated::Query_Unit::WORD_IN_ORDERSET, unit.tag());
  BOOST_REQUIRE_EQUAL("WORD_IN_ORDERSET", unit.text());

  const auto& default_unit = generated::Query::Unit::default_instance();
  BOOST_REQUIRE_EQUAL(default_unit.position(), unit.position());
  BOOST_REQUIRE_EQUAL(default_unit.frequency(), unit.frequency());
  BOOST_REQUIRE_EQUAL(default_unit.quantile(), unit.quantile());
  BOOST_REQUIRE_EQUAL(default_unit.pruning(), unit.pruning());
}

BOOST_AUTO_TEST_CASE(test_ends_with) {
  generated::Query query;
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::ASTERISK));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::DICTSET));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::OPTIONSET));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::ORDERSET));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::QMARK));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::QMARK_FOR_ASTERISK));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::WORD));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::WORD_IN_DICTSET));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::WORD_IN_OPTIONSET));
  BOOST_REQUIRE(!ends_with(query, generated::Query::Unit::WORD_IN_ORDERSET));

  append(query, make_unit(generated::Query::Unit::ASTERISK));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::ASTERISK));

  append(query, make_unit(generated::Query::Unit::DICTSET));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::DICTSET));

  append(query, make_unit(generated::Query::Unit::OPTIONSET));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::OPTIONSET));

  append(query, make_unit(generated::Query::Unit::ORDERSET));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::ORDERSET));

  append(query, make_unit(generated::Query::Unit::QMARK));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::QMARK));

  append(query, make_unit(generated::Query::Unit::QMARK_FOR_ASTERISK));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::QMARK_FOR_ASTERISK));

  append(query, make_unit(generated::Query::Unit::WORD));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::WORD));

  append(query, make_unit(generated::Query::Unit::WORD_IN_DICTSET));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::WORD_IN_DICTSET));

  append(query, make_unit(generated::Query::Unit::WORD_IN_OPTIONSET));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::WORD_IN_OPTIONSET));

  append(query, make_unit(generated::Query::Unit::WORD_IN_ORDERSET));
  BOOST_REQUIRE(ends_with(query, generated::Query::Unit::WORD_IN_ORDERSET));
}

BOOST_AUTO_TEST_CASE(test_starts_with) {
  generated::Query query;
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::ASTERISK));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::DICTSET));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::OPTIONSET));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::ORDERSET));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::QMARK));
  BOOST_REQUIRE(
      !starts_with(query, generated::Query::Unit::QMARK_FOR_ASTERISK));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::WORD));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::WORD_IN_DICTSET));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::WORD_IN_OPTIONSET));
  BOOST_REQUIRE(!starts_with(query, generated::Query::Unit::WORD_IN_ORDERSET));

  const std::vector<generated::Query::Unit> units = {
    make_unit(generated::Query::Unit::ASTERISK),
    make_unit(generated::Query::Unit::DICTSET),
    make_unit(generated::Query::Unit::OPTIONSET),
    make_unit(generated::Query::Unit::ORDERSET),
    make_unit(generated::Query::Unit::QMARK),
    make_unit(generated::Query::Unit::QMARK_FOR_ASTERISK),
    make_unit(generated::Query::Unit::WORD),
    make_unit(generated::Query::Unit::WORD_IN_DICTSET),
    make_unit(generated::Query::Unit::WORD_IN_OPTIONSET),
    make_unit(generated::Query::Unit::WORD_IN_ORDERSET)
  };

  append(query, make_unit(generated::Query::Unit::ASTERISK));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::ASTERISK));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::DICTSET));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::DICTSET));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::OPTIONSET));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::OPTIONSET));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::ORDERSET));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::ORDERSET));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::QMARK));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::QMARK));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::QMARK_FOR_ASTERISK));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::QMARK_FOR_ASTERISK));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::WORD));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::WORD));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::WORD_IN_DICTSET));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::WORD_IN_DICTSET));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::WORD_IN_OPTIONSET));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::WORD_IN_OPTIONSET));

  query.Clear();
  append(query, make_unit(generated::Query::Unit::WORD_IN_ORDERSET));
  append(query, units.begin(), units.end());
  BOOST_REQUIRE(starts_with(query, generated::Query::Unit::WORD_IN_ORDERSET));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace netspeak
