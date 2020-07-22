#include <boost/test/unit_test.hpp>

#include "netspeak/internal/Phrase.hpp"

namespace netspeak {

BOOST_AUTO_TEST_SUITE(test_phrase)

BOOST_AUTO_TEST_CASE(test_sort) {
  // sorted by descending frequency and ascending id
  internal::Phrase a(internal::Phrase::Id(3, 1), 1000);
  internal::Phrase b(internal::Phrase::Id(123, 3), 100);
  internal::Phrase c(internal::Phrase::Id(124, 3), 100);
  internal::Phrase d(internal::Phrase::Id(125, 3), 90);
  internal::Phrase e(internal::Phrase::Id(3, 4), 30);

  std::vector<internal::Phrase> expected{ a, b, c, d, e };

  std::vector<internal::Phrase> actual{ e, d, c, b, a };
  std::sort(actual.begin(), actual.end());

  BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expected.begin(),
                                expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace netspeak
