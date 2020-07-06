#include "netspeak/internal/Words.hpp"


namespace netspeak {
namespace internal {

Words::Words() : data_() {}

bool Words::is_subsequence_of(const Words& words) const {
  return is_subsequence_of(words.data());
}
bool Words::is_subsequence_of(const std::vector<const Word>& words) const {
  for (auto it = words.begin(); it != words.end(); ++it) {
    if (size() > std::distance(it, words.end()))
      break;

    if (std::equal(begin(), end(), it))
      return true;
  }

  return false;
}

std::ostream& operator<<(std::ostream& out, const Words& words) {
  const auto& data = words.data();

  auto it = data.begin();
  auto end = data.end();
  if (it == end) {
    out << "<empty>";
  } else {
    out << '"' << *it << '"';
    it++;
    for (; it != end; it++) {
      out << " " << '"' << *it << '"';
    }
  }

  return out;
}

} // namespace internal
} // namespace netspeak
