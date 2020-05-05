#ifndef NETSPEAK_JNI_JNI_UTILS_HPP
#define NETSPEAK_JNI_JNI_UTILS_HPP

#include <map>
#include <string>

#include "jni.h"

#include "netspeak/Configurations.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"

namespace netspeak {

const Configurations::Map to_netspeak_config(
    const generated::Properties& properties);

const generated::Properties to_properties(
    const std::map<std::string, std::string>& pairs);

} // namespace netspeak

#endif // NETSPEAK_JNI_JNI_UTILS_HPP
