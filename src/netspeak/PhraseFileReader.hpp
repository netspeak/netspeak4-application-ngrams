#ifndef NETSPEAK_PHRASE_FILE_READER_HPP
#define NETSPEAK_PHRASE_FILE_READER_HPP

#include "aitools/invertedindex/RecordReader.hpp"

#include "netspeak/PhraseFileParser.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/typedefs.hpp"

namespace netspeak {

/**
 * Parses text files in the ngram-file format, which defines a line as follows:
 * <ngram> TAB <frequency>
 *
 * Optionally there can be an additional value per line to assign some phrase
 * id (to set as template parameter). The line format then reads as follows:
 * <ngram> TAB <frequency> TAB <id>
 */
template <bool stream_provides_phrase_id>
class PhraseFileReader
    : public aitools::invertedindex::RecordReader<PhraseIndexValue> {
public:
  typedef aitools::invertedindex::RecordReader<PhraseIndexValue> base_type;
  typedef typename base_type::record_type record_type;

  PhraseFileReader(std::istream& is) : base_type(), wordpos_(), parser_(is) {}

  PhraseFileReader(std::istream& is, PhraseIndexValue::e2_type id_offset)
      : base_type(), wordpos_(), parser_(is, id_offset) {}

  virtual ~PhraseFileReader() {}

  //  inline bool
  //  good() const
  //  {
  //    return parser_.good();
  //  }

  virtual bool read(record_type& record) {
    if (phrase_.word_size() == 0 ||
        phrase_.word_size() == static_cast<int>(wordpos_)) {
      if (!parser_.read_next(phrase_))
        return false;
      wordpos_ = 0;
    }
    // <key> = <phrase-length>:<word-position>_<single-word>
    record.key().assign(aitools::to_string(phrase_.word_size()));
    record.key().push_back(':');
    record.key().append(aitools::to_string(wordpos_));
    record.key().push_back('_');
    record.key().append(phrase_.word(wordpos_).text());
    // <value> = (<phrase-frequency>, <phrase-id>)
    record.value().set_e1(phrase_.frequency());
    record.value().set_e2(phrase_.id());
    ++wordpos_;
    return true;
  }

  virtual std::size_t tell() {
    return parser_.tellg();
  }

private:
  size_t wordpos_;
  generated::Phrase phrase_;
  PhraseFileParser<stream_provides_phrase_id> parser_;
};

} // namespace netspeak

#endif // NETSPEAK_PHRASE_FILE_READER_HPP
