#ifndef NETSPEAK_PROTOBUF_UTILS_HPP
#define NETSPEAK_PROTOBUF_UTILS_HPP

#include <google/protobuf/repeated_field.h>

namespace netspeak {

// Primary template [ T = google::protobuf::Message ]
template <typename InputIterator, typename T>
google::protobuf::RepeatedPtrField<T>* copy(
    InputIterator first, InputIterator last,
    google::protobuf::RepeatedPtrField<T>* result) {
  while (first != last) {
    result->Add()->CopyFrom(*first);
    ++first;
  }
  return result;
}

// Partial specialized template [ T = std::string ]
template <typename InputIterator>
google::protobuf::RepeatedPtrField<std::string>* copy(
    InputIterator first, InputIterator last,
    google::protobuf::RepeatedPtrField<std::string>* result) {
  while (first != last) {
    result->Add()->assign(*first);
    ++first;
  }
  return result;
}

template <typename ContainerT, typename T>
google::protobuf::RepeatedPtrField<T>* copy(
    ContainerT container, google::protobuf::RepeatedPtrField<T>* result) {
  return copy(container.begin(), container.end(), result);
}

// Primary template [ T = google::protobuf::Message ]
template <typename InputIterator, typename T>
google::protobuf::RepeatedPtrField<T>* copy_n(
    InputIterator first, InputIterator last, size_t n,
    google::protobuf::RepeatedPtrField<T>* result) {
  while (first != last && n != 0) {
    result->Add()->CopyFrom(*first);
    ++first;
    --n;
  }
  return result;
}

// Partial specialized template [ T = std::string ]
template <typename InputIterator>
google::protobuf::RepeatedPtrField<std::string>* copy_n(
    InputIterator first, InputIterator last, size_t n,
    google::protobuf::RepeatedPtrField<std::string>* result) {
  while (first != last && n != 0) {
    result->Add()->assign(*first);
    ++first;
    --n;
  }
  return result;
}

} // namespace netspeak

#endif // NETSPEAK_PROTOBUF_UTILS_HPP
