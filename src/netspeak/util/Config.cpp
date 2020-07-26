#include "netspeak/util/Config.hpp"

#include "netspeak/util/PropertiesFormat.hpp"


namespace netspeak {
namespace util {

void Config::merge_properties(std::istream& in) {
  util::read_properties(in, std::inserter(data_, data_.end()));
}

std::ostream& Config::operator<<(std::ostream& out) const {
  util::write_properties(out, begin(), end());
  return out;
}

} // namespace util
} // namespace netspeak
