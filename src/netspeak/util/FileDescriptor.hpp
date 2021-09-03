#ifndef NETSPEAK_UTIL_FILE_DESCRIPTOR_HPP
#define NETSPEAK_UTIL_FILE_DESCRIPTOR_HPP

#include <sys/stat.h>

#include <memory>


namespace netspeak {
namespace util {

/**
 * @brief A managed file descriptor. This is implemented using a shared pointer.
 */
struct FileDescriptor final {
private:
  struct _fd {
    int fd;
    _fd() = delete;
    _fd(const _fd&) = delete;
    _fd(int fd);
    ~_fd();
  };
  std::shared_ptr<_fd> fd_;

public:
  FileDescriptor() = delete;
  FileDescriptor(int fd);

  static FileDescriptor open(const std::string& path, int __oflag);

  struct stat stat() const;

  operator int() const;
};


} // namespace util
} // namespace netspeak


#endif
