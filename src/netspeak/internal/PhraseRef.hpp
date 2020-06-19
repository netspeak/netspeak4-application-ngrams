#ifndef NETSPEAK_INTERNAL_PHRASE_REF_HPP
#define NETSPEAK_INTERNAL_PHRASE_REF_HPP

namespace netspeak {
namespace internal {

struct PhraseRef {
public:
  typedef uint32_t RefId;
  typedef uint32_t Length;
  typedef uint32_t Frequency;

  /**
   * The id of the phrase reference within a specific n-gram class.
   *
   * The n-gram class is given by the length of the phrase.
   */
  RefId id;
  /**
   * The number of words in the phrase.
   */
  Length length;
  /**
   * A bounded frequency of the referenced phrase.
   *
   * This frequency is bounded by the underlying integer type and may not be the
   * exact frequency. Only very few phrases (most only stop words) will have
   * frequencies great enough to be affect by this.
   */
  Frequency freq;

  PhraseRef(RefId id, Length length, Frequency freq)
      : id(id), length(length), freq(freq) {}
};


} // namespace internal
} // namespace netspeak


#endif
