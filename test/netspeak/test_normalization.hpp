#ifndef TEST_NORMALIZARION_HPP
#define TEST_NORMALIZARION_HPP


#include <dirent.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <unordered_map>

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include "aitools/util/check.hpp"

#include "antlr4-runtime.h"
#include "antlr4/QueryErrorHandler.hpp"
#include "antlr4/generated/QueryLexer.h"
#include "antlr4/generated/QueryParser.h"

#include "netspeak/Dictionaries.hpp"
#include "netspeak/error.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/query_methods.hpp"
#include "netspeak/query_normalization.hpp"


std::string normalize_spaces(const std::string& input);

/**
 * Compares two queryvectors if there elements are equal on a  string based
 * format.
 * @brief test_proof
 * @param normQuery a normalized query builded by the netspeak algorithms.
 * @param expectedQuery the normalized query as it is expected to be.
 */
void test_proof(std::vector<netspeak::generated::Query> normQuery,
                std::vector<netspeak::generated::Query> expectedQuery);

// generates the normalized queries from a word and a questionmark
const std::vector<netspeak::generated::Query> word_qmark();

// generates the normalized queries from a regular expression and a questionmark
const std::vector<netspeak::generated::Query> regexword_qmark();

// generates the normalized queries from a word and an asterisk
const std::vector<netspeak::generated::Query> word_asterisk();

// generates the normalized queries from a regular expression and an asterisk
const std::vector<netspeak::generated::Query> regexword_asterisk();

// generates the normalized queries from a word and a plus
const std::vector<netspeak::generated::Query> word_plus();

// generates the normalized queries from a regular expression and a plus
const std::vector<netspeak::generated::Query> regexword_plus();

// generates the normalized queries from a word and an optionset with one word
const std::vector<netspeak::generated::Query> word_optionset_one_word();

// generates the normalized queries from a word and an optionset with one
// regular expression
const std::vector<netspeak::generated::Query> word_optionset_one_regexword();

// generates the normalized queries from a word and an optionset with two words
const std::vector<netspeak::generated::Query> word_optionset_two_words();

// generates the normalized queries from a word and an optionset with two
// regular expressions
const std::vector<netspeak::generated::Query> word_optionset_two_regexwords();

// generates the normalized queries from a word and an optionset with one phrase
// filled with words
const std::vector<netspeak::generated::Query> word_optionset_one_phrase_words();

// generates the normalized queries from a word and an optionset with one phrase
// filled with regular expressions
const std::vector<netspeak::generated::Query>
word_optionset_one_phrase_regexwords();

// generates the normalized queries from a word and an optionset with one phrase
// and some words
const std::vector<netspeak::generated::Query> word_optionset_phrase_words();

// generates the normalized queries from a word and an optionset with one phrase
// and a regular expression
const std::vector<netspeak::generated::Query> word_optionset_phrase_regexword();

// generates the normalized queries from a word and an orderset of words
const std::vector<netspeak::generated::Query> word_orderset_words();

// generates the normalized queries from a word and an orderset of regular
// expressions
const std::vector<netspeak::generated::Query> word_orderset_regexwords();

// generates the normalized queries from a word and an orderset with a phrase
// and a word
const std::vector<netspeak::generated::Query> word_orderset_phrase_word();

// generates the normalized queries from a word and an orderset with a phrase
// and a regular expression
const std::vector<netspeak::generated::Query> word_orderset_phrase_regexword();

// generates the normalized queries from a word and a dictset operator bundled
// with a word
const std::vector<netspeak::generated::Query> word_dictset_word();

#endif
