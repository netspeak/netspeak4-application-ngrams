#ifndef NETSPEAK_PHRASE_FILE_PARSER_HPP
#define NETSPEAK_PHRASE_FILE_PARSER_HPP

#include <istream>

//#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/phrase_methods.hpp"
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
template <bool stream_provides_phrase_id> class PhraseFileParser {
public:
  PhraseFileParser(std::istream& is) : is_(is), id_offset_(0) {}

  PhraseFileParser(std::istream& is, PhraseId id_offset)
      : is_(is), id_offset_(id_offset) {}

  virtual ~PhraseFileParser() {}

  bool good() const {
    return is_.good();
  }

  size_t tellg() {
    return is_.tellg();
  }

  bool read_next(generated::Phrase& phrase) {
    parse_from<stream_provides_phrase_id>(phrase, is_);
    if (!stream_provides_phrase_id) {
      phrase.set_id(++id_offset_);
    }
    return is_.good();
  }

private:
  std::istream& is_;
  PhraseId id_offset_;
};

} // namespace netspeak

#endif // NETSPEAK_PHRASE_FILE_PARSER_HPP
