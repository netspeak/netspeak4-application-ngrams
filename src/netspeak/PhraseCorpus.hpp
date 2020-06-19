#ifndef NETSPEAK_PHRASE_CORPUS_HPP
#define NETSPEAK_PHRASE_CORPUS_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/filesystem.hpp>

#include "aitools/invertedindex/ByteBuffer.hpp"

#include "netspeak/internal/Phrase.hpp"
#include "netspeak/typedefs.hpp"

namespace netspeak {

using namespace internal;

/**
 * A corpus that, given the id of a phrase, retrieves the text (= list of words)
 * and the frequency of a phrase.
 *
 * The retrieved frequency is the actual frequency and not some modified proxy.
 */
class PhraseCorpus {
public:
  static const std::string txt_dir;
  static const std::string bin_dir;
  static const std::string vocab_file;
  static const std::string phrase_file;

  PhraseCorpus();

  PhraseCorpus(const boost::filesystem::path& phrase_dir);

  ~PhraseCorpus();

  bool is_open() const;

  void open(const boost::filesystem::path& phrase_dir);

  void close();

  /**
   * Returns the number of phrases with the given length.
   */
  Phrase::Count::Local count_phrases(Phrase::Id::Length phrase_len) const;
  /**
   * Returns the number of unique words in all phrases of any length.
   */
  size_t count_vocabulary() const;

  std::vector<Phrase> read_phrases(
      const std::vector<Phrase::Id>& phrase_ids) const;

private:
  Phrase decode_(const char* buffer, Phrase::Id phrase_id) const;

  void init_vocabulary_(const boost::filesystem::path& vocab_file);

  void open_phrase_files_(const boost::filesystem::path& phrase_dir);


  /**
   * @brief This maps the id of a word to its string value.
   */
  std::unordered_map<WordId, std::string> word_map;

  typedef int FileDescriptor;
  /**
   * @brief A map from the length of a phrase to the file descriptor of the
   * binary phrase corpus file that hold all phrase information for that length
   * (n-gram class).
   */
  std::unordered_map<Phrase::Id::Length, FileDescriptor> fd_map;
};

} // namespace netspeak

#endif // NETSPEAK_PHRASE_CORPUS_HPP
