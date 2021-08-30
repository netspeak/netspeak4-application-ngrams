#include "netspeak/util/logging.hpp"

#include <cstdarg>
#include <ctime>
#include <memory>
#include <mutex>

namespace netspeak {
namespace util {


static std::mutex log_mutex;

std::mutex& get_log_mutex() {
  return log_mutex;
}

std::ostream& print_timestamp_to(std::ostream& os) {
  if (os) {
    const time_t rawtime(std::time(NULL));
    struct tm* now(localtime(&rawtime));
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", now);
    os << timestr;
  }
  return os;
}

void log(const std::string& msg) {
  std::lock_guard<std::mutex> guard(get_log_mutex());
  print_timestamp_to(std::cout) << ' ' << msg << std::endl;
}
void log(const char* file, int line, const char* msg) {
  std::lock_guard<std::mutex> guard(get_log_mutex());
  print_timestamp_to(std::cout)
      << ' ' << file << ':' << line << ' ' << msg << std::endl;
}


} // namespace util
} // namespace netspeak
