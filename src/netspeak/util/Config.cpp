#include "netspeak/util/Config.hpp"

#include "netspeak/util/PropertiesFormat.hpp"
#include "netspeak/util/traceable_error.hpp"


namespace netspeak {
namespace util {


const std::string& Config::get(const std::string& key,
                               const std::string& default_value) const {
  if (contains(key)) {
    return data_.at(key);
  } else {
    return default_value;
  }
}
const std::string& Config::get(const std::string& key) const {
  if (contains(key)) {
    return data_.at(key);
  } else {
    if (file_name_.empty()) {
      throw tracable_runtime_error("Incomplete config. \"" + key +
                                   "\" is missing.");
    } else {
      throw tracable_runtime_error("Incomplete config. \"" + key +
                                   "\" is missing in \"" + file_name_ + "\".");
    }
  }
}


void Config::merge_properties(std::istream& in) {
  util::read_properties(in, std::inserter(data_, data_.end()));
}

std::ostream& Config::operator<<(std::ostream& out) const {
  util::write_properties(out, begin(), end());
  return out;
}

} // namespace util
} // namespace netspeak
