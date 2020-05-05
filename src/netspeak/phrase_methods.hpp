#ifndef NETSPEAK_PHRASE_METHODS_HPP
#define NETSPEAK_PHRASE_METHODS_HPP

#include <functional>
#include <ostream>
#include <sstream>

#include "netspeak/generated/NetspeakMessages.pb.h"

namespace std {

template <> struct hash<netspeak::generated::Phrase> {
  size_t operator()(const netspeak::generated::Phrase& phrase) const {
    return phrase.id();
  }
};

template <> struct equal_to<netspeak::generated::Phrase> {
  bool operator()(const netspeak::generated::Phrase& a,
                  const netspeak::generated::Phrase& b) const {
    return a.id() == b.id();
  }
};

} // namespace std

namespace netspeak {

bool is_subphrase_of(const generated::Phrase& a, const generated::Phrase& b);

template <bool stream_provides_phrase_id>
std::istream& parse_from(generated::Phrase& phrase, std::istream& is) {
  phrase.Clear();
  std::string line, word;
  std::string::size_type tabpos;
  while (std::getline(is, line)) {
    tabpos = line.find('\t');
    if (tabpos != std::string::npos) {
      std::istringstream iss(line.substr(0, tabpos));
      while (iss >> word) {
        phrase.add_word()->set_text(word);
      }
      iss.clear();
      iss.str(line.substr(tabpos));
      size_t number;
      iss >> number;
      phrase.set_frequency(number);
      if (stream_provides_phrase_id) {
        iss >> number;
        phrase.set_id(number);
      }
      return is;
    }
  }
  return is;
}

std::ostream& print_as_json_to(const generated::Phrase& phrase,
                               std::ostream& os);

std::ostream& print_as_json_to(const generated::Phrase::Word& word,
                               std::ostream& os);

std::ostream& print_as_text_to(const generated::Phrase& phrase,
                               std::ostream& os);

std::ostream& print_as_text_to(const generated::Phrase::Word& word,
                               std::ostream& os);

std::ostream& print_as_text_freq_id_to(const generated::Phrase& phrase,
                                       std::ostream& os);

std::ostream& println_as_text_to(const generated::Phrase& phrase,
                                 std::ostream& os);

std::ostream& println_as_text_to(const generated::Phrase::Word& word,
                                 std::ostream& os);

std::ostream& println_as_text_freq_id_to(const generated::Phrase& phrase,
                                         std::ostream& os);

uint64_t make_unique_id(size_t phrase_length, uint32_t phrase_id);

const std::string to_text(const generated::Phrase& phrase);

const std::string to_string(const generated::Phrase& phrase);

const char* to_c_str(const generated::Phrase& phrase);

const std::string to_json_string(const generated::Phrase& phrase);

// -----------------------------------------------------------------------------
// Operator support
// -----------------------------------------------------------------------------

namespace generated {
// Operators must be defined in the same namespace as its operands for
// argument-dependent lookup (ADL, also known as Koenig lookup).

bool operator==(const Phrase::Word& a, const Phrase::Word& b);

std::ostream& operator<<(std::ostream& os, const Phrase& phrase);

bool operator<(const Phrase& a, const Phrase& b);

bool operator>(const Phrase& a, const Phrase& b);

} // namespace generated
} // namespace netspeak

#endif // NETSPEAK_PHRASE_METHODS_HPP
