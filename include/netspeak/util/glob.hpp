#ifndef NETSPEAK_UTIL_GLOB_HPP
#define NETSPEAK_UTIL_GLOB_HPP

#include <string>
#include <vector>

namespace netspeak::util {

void glob(const std::string& pattern, std::vector<std::string>& paths);

}

#endif
