#ifndef NETSPEAK_UTIL_ID_MAP_HPP
#define NETSPEAK_UTIL_ID_MAP_HPP

#include <vector>

#include <boost/optional.hpp>

namespace netspeak {
namespace util {


/**
 * @brief Implements a simple map data structure where the key is some id that
 * will be used as the index of an array.
 *
 * This implementation will consume _O(M)_ memory where _M_ is the greatest key
 * used to set a value.
 *
 * @tparam Value
 */
template <typename Value>
class IdMap {
private:
  std::vector<boost::optional<Value>> data_;
  size_t size_;

  static inline const boost::optional<Value> EMPTY = boost::none;

public:
  IdMap() : data_(), size_(0) {}

  void set(size_t id, const Value& value) {
    if (id < data_.size()) {
      auto& place = data_[id];
      if (place == boost::none) {
        size_++;
      }
      place.emplace(value);
    } else {
      for (size_t i = data_.size(); i < id; i++) {
        data_.emplace_back(); // push boost::none
      }
      data_.emplace_back(value);
      size_++;
    }
  }

  const boost::optional<Value>& get(size_t id) const {
    if (id < data_.size()) {
      return data_[id];
    } else {
      return EMPTY;
    }
  }

  size_t size() const {
    return size_;
  }
  bool empty() const {
    return size_ == 0;
  }
};


} // namespace util
} // namespace netspeak

#endif
