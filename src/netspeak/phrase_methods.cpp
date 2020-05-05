#include "netspeak/phrase_methods.hpp"

#include <cstring>
#include <iostream>
#include <sstream>

namespace netspeak {

bool is_subphrase_of(const generated::Phrase& a, const generated::Phrase& b) {
  for (auto it = b.word().begin(); it != b.word().end(); ++it) {
    if (a.word_size() > std::distance(it, b.word().end()))
      break;
    if (std::equal(a.word().begin(), a.word().end(), it))
      return true;
  }
  return false;
}

std::ostream& print_as_json_to(const generated::Phrase& phrase,
                               std::ostream& os) {
  os << "{ \"id\" : " << phrase.id()
     << ", \"frequency\" : " << phrase.frequency() << ", \"word\" : [ ";
  for (auto it = phrase.word().begin(); it != phrase.word().end(); ++it) {
    if (it != phrase.word().begin()) {
      os << ", ";
    }
    print_as_json_to(*it, os);
  }
  return os << " ] }";
}

std::ostream& print_as_json_to(const generated::Phrase::Word& word,
                               std::ostream& os) {
  return os << "{ \"tag\" : \"" << generated::Phrase::Word::Tag_Name(word.tag())
            << "\", \"text\" : \"" << word.text() << "\" }";
}

std::ostream& print_as_text_to(const generated::Phrase& phrase,
                               std::ostream& os) {
  auto it = phrase.word().begin();
  if (it != phrase.word().end()) {
    print_as_text_to(*it, os);
  }
  for (++it; it != phrase.word().end(); ++it) {
    os << ' ';
    print_as_text_to(*it, os);
  }
  return os;
}

std::ostream& print_as_text_to(const generated::Phrase::Word& word,
                               std::ostream& os) {
  return os << word.text();
}

std::ostream& print_as_text_freq_id_to(const generated::Phrase& phrase,
                                       std::ostream& os) {
  return print_as_text_to(phrase, os)
         << '\t' << phrase.frequency() << '\t' << phrase.id();
}

std::ostream& println_as_text_to(const generated::Phrase& phrase,
                                 std::ostream& os) {
  return print_as_text_to(phrase, os) << '\n';
}

std::ostream& println_as_text_to(const generated::Phrase::Word& word,
                                 std::ostream& os) {
  return print_as_text_to(word, os) << '\n';
}

std::ostream& println_as_text_freq_id_to(const generated::Phrase& phrase,
                                         std::ostream& os) {
  return print_as_text_freq_id_to(phrase, os) << '\n';
}

uint64_t make_unique_id(size_t phrase_length, uint32_t phrase_id) {
  return phrase_length * (1ul << 40) + phrase_id;
}

const std::string to_text(const generated::Phrase& phrase) {
  std::ostringstream oss;
  for (auto it = phrase.word().begin(); it != phrase.word().end(); ++it) {
    if (it != phrase.word().begin()) {
      oss << ' ';
    }
    oss << it->text();
  }
  return oss.str();
}

const std::string to_string(const generated::Phrase& phrase) {
  std::ostringstream oss;
  print_as_text_freq_id_to(phrase, oss);
  return oss.str();
}

const char* to_c_str(const generated::Phrase& phrase) {
  return to_string(phrase).c_str();
}

const std::string to_json_string(const generated::Phrase& phrase) {
  std::ostringstream oss;
  print_as_json_to(phrase, oss);
  return oss.str();
}

namespace generated {

bool operator==(const Phrase::Word& a, const Phrase::Word& b) {
  return a.text() == b.text();
}

std::ostream& operator<<(std::ostream& os, const Phrase& phrase) {
  return print_as_text_freq_id_to(phrase, os);
}

bool operator<(const Phrase& a, const Phrase& b) {
  return a.frequency() < b.frequency();
}

bool operator>(const Phrase& a, const Phrase& b) {
  return a.frequency() > b.frequency();
}

} // namespace generated
} // namespace netspeak
