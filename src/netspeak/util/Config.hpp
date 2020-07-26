#ifndef NETSPEAK_UTIL_CONFIG_HPP
#define NETSPEAK_UTIL_CONFIG_HPP

#include <istream>
#include <map>
#include <ostream>
#include <string>

namespace netspeak {
namespace util {

/**
 * @brief A general configuration class backed by a simple map of string
 * key-value-pairs.
 *
 * It also contains functionality to import and export `.properties` files.
 */
class Config {
private:
  std::map<std::string, std::string> data_;

public:
  typedef std::map<std::string, std::string>::value_type initializer_list_type;
  Config(std::initializer_list<initializer_list_type> list) : data_(list) {}
  Config() : data_() {}

  std::map<std::string, std::string>::const_iterator begin() const {
    return data_.begin();
  }
  std::map<std::string, std::string>::const_iterator end() const {
    return data_.end();
  }
  std::map<std::string, std::string>::const_iterator find(
      const std::string& key) const {
    return data_.find(key);
  }

  bool contains(const std::string& key) const {
    return data_.find(key) != data_.end();
  }

  const std::string& at(const std::string& key) const {
    return data_.at(key);
  }

  bool empty() const {
    return data_.empty();
  }

  const std::string& get(const std::string& key,
                         const std::string& default_value) const {
    if (contains(key)) {
      return data_.at(key);
    } else {
      return default_value;
    }
  }

  std::string& operator[](const std::string& key) {
    return data_[key];
  }

  /**
   * @brief Inserts all key-value-pairs of the given `.properties` file into
   * this config overwriting existing keys.
   */
  void merge_properties(std::istream& in);
  /**
   * @brief Inserts all key-value-pairs of the given iterator file into this
   * config overwriting existing keys.
   */
  template <typename T> void merge(T _begin, T _end) {
    for (; _begin != _end; _begin++) {
      data_.insert(*_begin);
    }
  }
  /**
   * @brief This will output the config in the `.properties` file format.
   */
  std::ostream& operator<<(std::ostream& out) const;
};

} // namespace util
} // namespace netspeak

#endif
