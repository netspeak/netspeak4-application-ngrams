#ifndef TEST_NETSPEAK_MANAGED_DIRECTORY_HPP
#define TEST_NETSPEAK_MANAGED_DIRECTORY_HPP

#include <filesystem>
#include <string>

#include <boost/test/unit_test.hpp>

namespace test {

/**
 * @brief Each instance of this class owns a direcotry in the file system.
 *
 * The directory and all its contents will be removed when a instance goes
 * out of scope.
 */
class ManagedDirectory {
private:
  const std::filesystem::path path_;

public:
  ManagedDirectory() = delete;
  ManagedDirectory(const ManagedDirectory&) = delete;
  explicit ManagedDirectory(const std::string& path_str) : path_(path_str) {
    BOOST_REQUIRE(std::filesystem::create_directory(path_));
  }
  ~ManagedDirectory() {
    std::filesystem::remove_all(path_);
  }

  const std::filesystem::path& dir() const {
    return path_;
  }
};

} // namespace test

#endif
