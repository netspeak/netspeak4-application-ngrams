#include <netspeak/util/glob.hpp>

// https://stackoverflow.com/a/8615450/7595472

#include <glob.h>
#include <string.h>

#include <format>
#include <stdexcept>
#include <string>
#include <vector>

namespace netspeak::util {


void glob(const std::string& pattern, std::vector<std::string>& paths) {
  // glob struct resides on the stack
  glob_t glob_result;
  memset(&glob_result, 0, sizeof(glob_result));

  // do the glob operation
  int return_value = glob(pattern.c_str(), GLOB_TILDE, nullptr, &glob_result);
  if (return_value != 0) {
    globfree(&glob_result);
    throw std::runtime_error(std::format("glob() failed with return value {}", return_value));
  }

  // collect all the filenames into a std::list<std::string>
  for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
    paths.push_back(std::string(glob_result.gl_pathv[i]));
  }

  // cleanup
  globfree(&glob_result);
}

} // namespace netspeak::util