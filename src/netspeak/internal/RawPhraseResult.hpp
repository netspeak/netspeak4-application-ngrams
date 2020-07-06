#ifndef NETSPEAK_INTERNAL_RAW_PHRASE_RESULT_HPP
#define NETSPEAK_INTERNAL_RAW_PHRASE_RESULT_HPP

#include <string>
#include <vector>

#include "netspeak/internal/Phrase.hpp"


namespace netspeak {
namespace internal {


/**
 * @brief A result set of phrase of a single norm query.
 *
 * The set and its phrases do not hold a reference to the norm query that
 * created them. Since all phrases were created from a single norm query, all
 * phrases are guaranteed to have the same length.
 */
class RawPhraseResult {
private:
  std::vector<const Phrase> phrases_;
  std::vector<const std::string> unknown_words_;

public:
  /**
   * @brief The phrases of this result set sorted by frequency in descending
   * order.
   */
  std::vector<const Phrase>& phrases() {
    return phrases_;
  }
  const std::vector<const Phrase>& phrases() const {
    return phrases_;
  }

  /**
   * @brief Returns whether this result set contains no phrases.
   */
  bool empty() const {
    return phrases_.empty();
  }

  std::vector<const std::string>& unknown_words() {
    return unknown_words_;
  }
  const std::vector<const std::string>& unknown_words() const {
    return unknown_words_;
  }
};


} // namespace internal
} // namespace netspeak


#endif
