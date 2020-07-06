#ifndef NETSPEAK_UTIL_VEC_HPP
#define NETSPEAK_UTIL_VEC_HPP

#include <vector>


namespace netspeak {
namespace util {

/**
 * @brief A simple function that appends all elements of \c other to \c base.
 *
 * @tparam T
 * @param base
 * @param other
 */
template <typename T, typename I>
inline void vec_append(std::vector<T>& base, const I& other) {
  base.insert(base.end(), other.begin(), other.end());
}

template <typename T>
inline bool vec_contains(std::vector<T>& base, const T& item) {
  const auto it = std::find(base.begin(), base.end(), item);
  return it != base.end();
}

/**
 * @brief Assuming a sorted vector, this will remove all duplicate elements in
 * the vector.
 *
 * Elements are compared using the standard != operator.
 */
template <typename T> void vec_sorted_filter_dups(std::vector<T>& base) {
  size_t write_i = 0;
  for (size_t read_i = 1; read_i != base.size(); read_i++) {
    if (base[read_i] != base[write_i]) {
      write_i++;
      base[write_i] = base[read_i];
    }
  }
  write_i++;
  base.erase(base.begin() + write_i);
}

} // namespace util
} // namespace netspeak


#endif
