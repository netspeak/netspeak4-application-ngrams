#include "netspeak/Configurations.hpp"

#include <string>

namespace netspeak {

const std::string Configurations::path_to_home("path.to.home");

const std::string Configurations::path_to_phrase_index("path.to.phrase.index");

const std::string Configurations::path_to_phrase_corpus(
    "path.to.phrase.corpus");

const std::string Configurations::path_to_phrase_dictionary(
    "path.to.phrase.dictionary");

const std::string Configurations::path_to_postlist_index(
    "path.to.postlist.index");

const std::string Configurations::path_to_hash_dictionary(
    "path.to.hash.dictionary");

const std::string Configurations::path_to_regex_vocabulary(
    "path.to.regex.vocabulary");

const std::string Configurations::cache_capacity("cache.capacity");

const std::string Configurations::default_phrase_index_dir_name("phrase-index");

const std::string Configurations::default_phrase_corpus_dir_name(
    "phrase-corpus");

const std::string Configurations::default_phrase_dictionary_dir_name(
    "phrase-dictionary");

const std::string Configurations::default_postlist_index_dir_name(
    "postlist-index");

const std::string Configurations::default_hash_dictionary_dir_name(
    "hash-dictionary");

const std::string Configurations::default_regex_vocabulary_dir_name(
    "regex-vocabulary");

bool Configurations::contains(const Map& config, const std::string& key) {
  return config.find(key) != config.end();
}

} // namespace netspeak
