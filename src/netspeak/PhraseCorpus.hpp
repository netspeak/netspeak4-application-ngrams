#ifndef NETSPEAK_PHRASE_CORPUS_HPP
#define NETSPEAK_PHRASE_CORPUS_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/filesystem.hpp>

#include "aitools/invertedindex/ByteBuffer.hpp"

#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/typedefs.hpp"

namespace netspeak {

/**
 * A class to read textual phrases (n-grams) by their id representation.
 */
class PhraseCorpus {
private:
  typedef std::unordered_map<size_t, std::string> word_id_to_word_str;
  typedef std::unordered_map<size_t, size_t> phrase_len_to_encoded_size;
  typedef std::unordered_map<size_t, int> phrase_len_to_file_des;

public:
  static const std::string txt_dir;
  static const std::string bin_dir;
  static const std::string vocab_file;
  static const std::string phrase_file;

  PhraseCorpus();

  PhraseCorpus(const boost::filesystem::path& phrase_dir);

  ~PhraseCorpus();

  void close();

  /**
   * Returns the number of phrases with the given length.
   */
  size_t count_phrases(size_t phrase_len) const;
  /**
   * Returns the number of unique words in all phrases of any length.
   */
  size_t count_vocabulary() const;

  bool is_open() const;

  void open(const boost::filesystem::path& phrase_dir);

  generated::Phrase read_phrase(size_t phrase_len, size_t phrase_id) const;

  generated::Phrase read_phrase(
      const generated::QueryResult::PhraseRef& phrase_ref) const;

  std::vector<generated::Phrase> read_phrases(
      const std::vector<std::pair<size_t, size_t> >& refs) const;

  std::vector<generated::Phrase> read_phrases(
      const std::vector<generated::QueryResult::PhraseRef>& phrase_refs) const;

private:
  generated::Phrase decode_(aitools::invertedindex::ByteBuffer& buffer,
                            size_t phrase_id = 0) const;

  void init_vocabulary_(const boost::filesystem::path& vocab_file);

  void open_phrase_files_(const boost::filesystem::path& phrase_dir);

  word_id_to_word_str word_id_to_word_str_;
  phrase_len_to_encoded_size phrase_len_to_encoded_size_;
  phrase_len_to_file_des phrase_len_to_file_des_;
};

} // namespace netspeak

#endif // NETSPEAK_PHRASE_CORPUS_HPP
