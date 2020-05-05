#include "netspeak/Properties.hpp"

namespace netspeak {

// cache properties
const std::string Properties::cache_size("cache.size");

const std::string Properties::cache_policy("cache.policy");

const std::string Properties::cache_capacity("cache.capacity");

const std::string Properties::cache_access_count("cache.access.count");

const std::string Properties::cache_hit_rate("cache.hit.rate");

const std::string Properties::cache_top_100("cache.top.100");

// phrase corpus properties
const std::string Properties::phrase_corpus_1gram_count(
    "phrase.corpus.1gram.count");

const std::string Properties::phrase_corpus_2gram_count(
    "phrase.corpus.2gram.count");

const std::string Properties::phrase_corpus_3gram_count(
    "phrase.corpus.3gram.count");

const std::string Properties::phrase_corpus_4gram_count(
    "phrase.corpus.4gram.count");

const std::string Properties::phrase_corpus_5gram_count(
    "phrase.corpus.5gram.count");

// phrase dictionary properties
const std::string Properties::phrase_dictionary_size("phrase.dictionary.size");

const std::string Properties::phrase_dictionary_value_type(
    "phrase.dictionary.value.type");

// phrase index properties
const std::string Properties::phrase_index_key_count("phrase.index.key.count");

const std::string Properties::phrase_index_value_type(
    "phrase.index.value.type");

const std::string Properties::phrase_index_value_sorting(
    "phrase.index.value.sorting");

const std::string Properties::phrase_index_value_count(
    "phrase.index.value.count");

const std::string Properties::phrase_index_total_size(
    "phrase.index.total.size");

// postlist index properties
const std::string Properties::postlist_index_key_count(
    "postlist.index.key.count");

const std::string Properties::postlist_index_value_type(
    "postlist.index.value.type");

const std::string Properties::postlist_index_value_sorting(
    "postlist.index.value.sorting");

const std::string Properties::postlist_index_value_count(
    "postlist.index.value.count");

const std::string Properties::postlist_index_total_size(
    "postlist.index.total.size");

// hash dictionary properties
const std::string Properties::hash_dictionary_size("hash.dictionary.size");

const std::string Properties::hash_dictionary_value_type(
    "hash.dictionary.value.type");

// regex vocabulary properties
const std::string Properties::regex_vocabulary_size("regex.vocabulary.size");

const std::string Properties::regex_vocabulary_value_type(
    "regex.vocabulary.value.type");

bool Properties::contains(const Map& config, const std::string& key) {
  return config.find(key) != config.end();
}

} // namespace netspeak
