#include "netspeak/util/FileDescriptor.hpp"

#include <fcntl.h>
#include <unistd.h>

#include "netspeak/error.hpp"
#include "netspeak/util/check.hpp"


namespace netspeak {
namespace util {

FileDescriptor::_fd::_fd(int fd) : fd(fd) {}
FileDescriptor::_fd::~_fd() {
  ::close(fd);
}

FileDescriptor::FileDescriptor(int fd) : fd_() {
  if (fd == -1) {
    throw std::runtime_error("Invalid file descriptor.");
  }
  fd_ = std::make_shared<_fd>(fd);
}

FileDescriptor::operator int() const {
  return fd_->fd;
}


FileDescriptor FileDescriptor::open(const std::string& path, int __oflag) {
  int fd = ::open(path.c_str(), __oflag);
  util::check(fd != -1, error_message::cannot_open, path);
  return FileDescriptor(fd);
}

struct stat FileDescriptor::stat() const {
  struct stat stats;
  int err = ::fstat(*this, &stats);
  util::check(err != -1, error_message::does_not_exist);
  return stats;
}


} // namespace util
} // namespace netspeak
