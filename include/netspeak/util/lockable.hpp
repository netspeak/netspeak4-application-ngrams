#ifndef NETSPEAK_UTIL_LOCKABLE_HPP
#define NETSPEAK_UTIL_LOCKABLE_HPP

#include <mutex>

namespace netspeak::util {

/**
 * @brief Allows locking and unlocking the specified member variable.
 * @details Allows locking and unlocking the specified member variable.
 *
 * @tparam T
 */
template <class T>
class Lockable {
private:
  T value;
  std::mutex mutex;

  /** @brief Deleted; The copy instruction may require read-access when other.mutex is locked. **/
  Lockable(const Lockable& other) = delete;
  /** @brief Deleted; Mutexes are immovable. **/
  Lockable(Lockable&& other) = delete;
  /** @brief Deleted; The assignemt may require read-access when other.mutex is locked or write-access when this->mutex
   * is locked. **/
  Lockable& operator=(const Lockable& other) = delete;
  /** @brief Deleted; Mutexes are immovable. **/
  Lockable& operator=(Lockable&& other) = delete;

public:
  Lockable() = default;
  explicit Lockable(const T& value) : value(value), mutex() {}
  explicit Lockable(T&& value) : value(std::move(value)), mutex() {}

  /**
   * @brief Returns a shared_ptr to the unlocked resource. The resource remains unlocked as long as it is referenced
   * using a shared_ptr!
   * @details Returns a shared_ptr to the unlocked resource. The resource remains unlocked as long as it is referenced
   * using a shared_ptr! As such the value returned by lock() should be short lived and the shared_ptr should not live
   * longer than the instance that created it.
   *
   * @return std::shared_ptr<T>
   */
  std::shared_ptr<T> lock() {
    auto counter = std::make_shared<std::lock_guard<std::mutex>>(std::ref(mutex));
    return std::shared_ptr<T>{ counter, &value };
  }
};

} // namespace netspeak::util

#endif
