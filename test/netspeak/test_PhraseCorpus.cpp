#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include "paths.hpp"

#include "netspeak/PhraseCorpus.hpp"
#include "netspeak/query_methods.hpp"


namespace netspeak {

BOOST_AUTO_TEST_SUITE(test_PhraseCorpus)

const boost::filesystem::path phrase_files_dir(test::INDEX_DIR +
                                               "/phrase-corpus/bin");

// the test index is fixed, so we know these numebrs
const int INDEX_VOCAB_COUNT = 61741;
const int INDEX_1GRAM_COUNT = 61738;
const int INDEX_2GRAM_COUNT = 154797;
const int INDEX_3GRAM_COUNT = 77687;
const int INDEX_4GRAM_COUNT = 20569;
const int INDEX_5GRAM_COUNT = 7271;

BOOST_AUTO_TEST_CASE(test_open) {
  PhraseCorpus corpus;

  BOOST_REQUIRE_THROW(corpus.open(phrase_files_dir / "#"), std::runtime_error);
  BOOST_REQUIRE_NO_THROW(corpus.open(phrase_files_dir));
}

BOOST_AUTO_TEST_CASE(test_count_phrases) {
  PhraseCorpus corpus(phrase_files_dir);

  // this is trivially true for all indexes
  BOOST_REQUIRE_EQUAL(0, corpus.count_phrases(0));

  // we removed all phrases with > 5 words
  BOOST_REQUIRE_EQUAL(0, corpus.count_phrases(6));
  BOOST_REQUIRE_EQUAL(0, corpus.count_phrases(7));
  BOOST_REQUIRE_EQUAL(0, corpus.count_phrases(8));

  // We just need to see that there are some phrases
  BOOST_REQUIRE_EQUAL(INDEX_1GRAM_COUNT, corpus.count_phrases(1));
  BOOST_REQUIRE_EQUAL(INDEX_2GRAM_COUNT, corpus.count_phrases(2));
  BOOST_REQUIRE_EQUAL(INDEX_3GRAM_COUNT, corpus.count_phrases(3));
  BOOST_REQUIRE_EQUAL(INDEX_4GRAM_COUNT, corpus.count_phrases(4));
  BOOST_REQUIRE_EQUAL(INDEX_5GRAM_COUNT, corpus.count_phrases(5));
}

BOOST_AUTO_TEST_CASE(test_count_vocabulary) {
  PhraseCorpus corpus(phrase_files_dir);

  // this is trivially true for all indexes
  BOOST_REQUIRE_EQUAL(INDEX_VOCAB_COUNT, corpus.count_vocabulary());
}

BOOST_AUTO_TEST_CASE(test_read_phrase) {
  PhraseCorpus corpus(phrase_files_dir);

  // invalid ngram_len
  BOOST_REQUIRE_THROW(corpus.read_phrase(0, 0), std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(6, 0), std::runtime_error);

  // invalid ngram_id
  BOOST_REQUIRE_THROW(corpus.read_phrase(1, -1), std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(2, -1), std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(3, -1), std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(4, -1), std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(5, -1), std::runtime_error);

  // invalid ngram_id
  BOOST_REQUIRE_THROW(corpus.read_phrase(1, INDEX_1GRAM_COUNT),
                      std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(2, INDEX_2GRAM_COUNT),
                      std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(3, INDEX_3GRAM_COUNT),
                      std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(4, INDEX_4GRAM_COUNT),
                      std::runtime_error);
  BOOST_REQUIRE_THROW(corpus.read_phrase(5, INDEX_5GRAM_COUNT),
                      std::runtime_error);

  generated::Phrase phrase;

  // valid ngram_len and ngram_id
  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(1, 0));
  BOOST_REQUIRE_EQUAL(1, phrase.word_size());
  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(1, INDEX_1GRAM_COUNT - 1));
  BOOST_REQUIRE_EQUAL(1, phrase.word_size());

  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(2, 0));
  BOOST_REQUIRE_EQUAL(2, phrase.word_size());
  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(2, INDEX_2GRAM_COUNT - 1));
  BOOST_REQUIRE_EQUAL(2, phrase.word_size());

  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(3, 0));
  BOOST_REQUIRE_EQUAL(3, phrase.word_size());
  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(3, INDEX_3GRAM_COUNT - 1));
  BOOST_REQUIRE_EQUAL(3, phrase.word_size());

  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(4, 0));
  BOOST_REQUIRE_EQUAL(4, phrase.word_size());
  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(4, INDEX_4GRAM_COUNT - 1));
  BOOST_REQUIRE_EQUAL(4, phrase.word_size());

  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(5, 0));
  BOOST_REQUIRE_EQUAL(5, phrase.word_size());
  BOOST_REQUIRE_NO_THROW(phrase = corpus.read_phrase(5, INDEX_5GRAM_COUNT - 1));
  BOOST_REQUIRE_EQUAL(5, phrase.word_size());
}

BOOST_AUTO_TEST_CASE(test_read_phrases) {
  PhraseCorpus corpus(phrase_files_dir);
  std::vector<std::pair<size_t, size_t>> refs;

  // invalid ngram_len
  refs = { { 0, 0 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);
  refs = { { 6, 0 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);

  // invalid ngram_id
  refs = { { 1, -1 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);
  refs = { { 2, -1 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);
  refs = { { 3, -1 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);
  refs = { { 4, -1 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);
  refs = { { 5, -1 } };
  BOOST_REQUIRE_THROW(corpus.read_phrases(refs), std::runtime_error);

  std::vector<generated::Phrase> phrases;

  // valid ngram_len = 1
  refs = { { 1, 0 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(1, phrases.size());
  BOOST_REQUIRE_EQUAL(1, phrases.front().word_size());
  refs = { { 1, 0 }, { 1, INDEX_1GRAM_COUNT - 1 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(2, phrases.size());
  BOOST_REQUIRE_EQUAL(1, phrases.front().word_size());
  BOOST_REQUIRE_EQUAL(1, phrases.back().word_size());

  // valid ngram_len = 2
  refs = { { 2, 0 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(1, phrases.size());
  BOOST_REQUIRE_EQUAL(2, phrases.front().word_size());
  refs = { { 2, 0 }, { 2, INDEX_2GRAM_COUNT - 1 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(2, phrases.size());
  BOOST_REQUIRE_EQUAL(2, phrases.front().word_size());
  BOOST_REQUIRE_EQUAL(2, phrases.back().word_size());

  // valid ngram_len = 3
  refs = { { 3, 0 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(1, phrases.size());
  BOOST_REQUIRE_EQUAL(3, phrases.front().word_size());
  refs = { { 3, 0 }, { 3, INDEX_3GRAM_COUNT - 1 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(2, phrases.size());
  BOOST_REQUIRE_EQUAL(3, phrases.front().word_size());
  BOOST_REQUIRE_EQUAL(3, phrases.back().word_size());

  // valid ngram_len = 4
  refs = { { 4, 0 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(1, phrases.size());
  BOOST_REQUIRE_EQUAL(4, phrases.front().word_size());
  refs = { { 4, 0 }, { 4, INDEX_4GRAM_COUNT - 1 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(2, phrases.size());
  BOOST_REQUIRE_EQUAL(4, phrases.front().word_size());
  BOOST_REQUIRE_EQUAL(4, phrases.back().word_size());

  // valid ngram_len = 5
  refs = { { 5, 0 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(1, phrases.size());
  BOOST_REQUIRE_EQUAL(5, phrases.front().word_size());
  refs = { { 5, 0 }, { 5, INDEX_5GRAM_COUNT - 1 } };
  BOOST_REQUIRE_NO_THROW(phrases = corpus.read_phrases(refs));
  BOOST_REQUIRE_EQUAL(2, phrases.size());
  BOOST_REQUIRE_EQUAL(5, phrases.front().word_size());
  BOOST_REQUIRE_EQUAL(5, phrases.back().word_size());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace netspeak
