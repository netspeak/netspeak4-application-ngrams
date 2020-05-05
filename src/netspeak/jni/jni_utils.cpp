#include "netspeak/jni/jni_utils.hpp"

namespace netspeak {

const Configurations::Map to_netspeak_config(
    const generated::Properties& properties) {
  Configurations::Map config;
  for (const auto& property : properties.property()) {
    config[property.key()] = property.value();
  }
  return config;
}

const generated::Properties to_properties(
    const std::map<std::string, std::string>& pairs) {
  generated::Properties properties;
  for (const auto& pair : pairs) {
    auto property = properties.add_property();
    property->set_key(pair.first);
    property->set_value(pair.second);
  }
  return properties;
}

} // namespace netspeak
