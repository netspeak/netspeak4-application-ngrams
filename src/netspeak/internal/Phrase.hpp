#ifndef NETSPEAK_INTERNAL_PHRASE_HPP
#define NETSPEAK_INTERNAL_PHRASE_HPP

#include <string>
#include <vector>
#include "netspeak/internal/Words.hpp"

namespace netspeak {
namespace internal {

/**
 * A data container for the global id of phrase.
 *
 * The global id of a phrase is the combination of the local id of the phrase (=
 * the id within a specific n-gram class) and the length of the phrase (= the
 * number of words = the n-gram class of the phrase).
 */
struct PhraseId_impl {
public:
  typedef uint32_t Local;
  typedef uint32_t Length;

  /**
   * @brief The maximum value a local id is allowed to have.
   */
  static const Local LOCAL_MAX;
  /**
   * @brief The maximum length supported.
   */
  static const Length LENGTH_MAX;

private:
  Length length_;
  Local local_;

public:
  PhraseId_impl(Length len, Local local_id)
      : length_(len), local_(local_id) {}

  Local local() const {
    return local_;
  }
  Length length() const {
    return length_;
  }

  operator uint64_t() const {
    return ((uint64_t)length_) << 32 | local_;
  }
};

struct Phrase {
public:
  typedef PhraseId_impl Id;
  typedef uint64_t Frequency;

  typedef struct {
    typedef uint64_t Global;
    typedef uint32_t Local;
    typedef uint32_t Length;
  } Count;

private:
  Id id_;
  Frequency freq_;
  Words words_;

public:
  Phrase(Id id, Frequency freq) : id_(id), freq_(freq), words_() {}

  Id id() const {
    return id_;
  }
  Frequency freq() const {
    return freq_;
  }
  const Words& words() const {
    return words_;
  }
  Words& words() {
    return words_;
  }
};

} // namespace internal
} // namespace netspeak


#endif
