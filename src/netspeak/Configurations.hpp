#ifndef NETSPEAK_CONFIGURATIONS_HPP
#define NETSPEAK_CONFIGURATIONS_HPP

#include <map>
#include <string>

namespace netspeak {

class Configurations {
public:
  static const std::string path_to_home;
  static const std::string path_to_phrase_index;
  static const std::string path_to_phrase_corpus;
  static const std::string path_to_phrase_dictionary;
  static const std::string path_to_postlist_index;
  static const std::string path_to_hash_dictionary;
  static const std::string path_to_regex_vocabulary;
  static const std::string cache_capacity;

  static const std::string default_phrase_index_dir_name;
  static const std::string default_phrase_corpus_dir_name;
  static const std::string default_phrase_dictionary_dir_name;
  static const std::string default_postlist_index_dir_name;
  static const std::string default_hash_dictionary_dir_name;
  static const std::string default_regex_vocabulary_dir_name;

  typedef std::map<std::string, std::string> Map;

  static bool contains(const Map& config, const std::string& key);
};

} // namespace netspeak

#endif // NETSPEAK_CONFIGURATIONS_HPP
