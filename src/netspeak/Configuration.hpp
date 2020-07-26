#ifndef NETSPEAK_CONFIGURATION_HPP
#define NETSPEAK_CONFIGURATION_HPP

#include "netspeak/util/Config.hpp"

namespace netspeak {

class Configuration : public util::Config {
public:
  Configuration() : util::Config() {}
  Configuration(std::initializer_list<util::Config::initializer_list_type> list)
      : util::Config(list) {}

public:
  static const std::string path_to_home;
  static const std::string path_to_phrase_index;
  static const std::string path_to_phrase_corpus;
  static const std::string path_to_phrase_dictionary;
  static const std::string path_to_postlist_index;
  static const std::string path_to_hash_dictionary;
  static const std::string path_to_regex_vocabulary;

  static const std::string cache_capacity;

  static const std::string corpus_key;
  static const std::string corpus_name;
  static const std::string corpus_language;

  // TODO: Should the default values really be here or should they rather be in
  // Netspeak.hpp?
  static const std::string default_phrase_index_dir_name;
  static const std::string default_phrase_corpus_dir_name;
  static const std::string default_phrase_dictionary_dir_name;
  static const std::string default_postlist_index_dir_name;
  static const std::string default_hash_dictionary_dir_name;
  static const std::string default_regex_vocabulary_dir_name;
};

} // namespace netspeak

#endif
