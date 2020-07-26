#include "netspeak/Configuration.hpp"

#include <string>

namespace netspeak {

#define PREFIX const std::string Configuration

PREFIX::path_to_home("path.to.home");
PREFIX::path_to_phrase_index("path.to.phrase.index");
PREFIX::path_to_phrase_corpus("path.to.phrase.corpus");
PREFIX::path_to_phrase_dictionary("path.to.phrase.dictionary");
PREFIX::path_to_postlist_index("path.to.postlist.index");
PREFIX::path_to_hash_dictionary("path.to.hash.dictionary");
PREFIX::path_to_regex_vocabulary("path.to.regex.vocabulary");

PREFIX::cache_capacity("cache.capacity");

PREFIX::default_phrase_index_dir_name("phrase-index");
PREFIX::default_phrase_corpus_dir_name("phrase-corpus");
PREFIX::default_phrase_dictionary_dir_name("phrase-dictionary");
PREFIX::default_postlist_index_dir_name("postlist-index");
PREFIX::default_hash_dictionary_dir_name("hash-dictionary");
PREFIX::default_regex_vocabulary_dir_name("regex-vocabulary");

} // namespace netspeak
