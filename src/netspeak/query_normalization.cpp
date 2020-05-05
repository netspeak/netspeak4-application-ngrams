#include "netspeak/query_normalization.hpp"

#include <list>
#include <string>

#include <boost/algorithm/string/join.hpp>

#include "aitools/util/check.hpp"

#include "antlr4-runtime.h"
#include "antlr4/ProxyListener.hpp"
#include "antlr4/QueryComplexityMeasureListener.hpp"
#include "antlr4/QueryErrorHandler.hpp"
#include "antlr4/QueryLengthVerifier.hpp"
#include "antlr4/QueryNormListener.hpp"
#include "antlr4/RawQueryListener.hpp"
#include "antlr4/generated/QueryLexer.h"
#include "antlr4/generated/QueryParser.h"

#include "netspeak/error.hpp"
#include "netspeak/query_methods.hpp"

namespace netspeak {

void parse(std::shared_ptr<generated::RawResponse> response,
           netspeak::Dictionaries::Map *dictSet,
           netspeak::regex::RegexIndex *regex_index,
           std::vector<netspeak::generated::Query> *normQueries) {
  using namespace antlr4;

  if (response->request().query().empty())
    return;

  ANTLRInputStream input(response->request().query());
  QueryLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  QueryParser parser(&tokens);
  parser.removeErrorListeners();
  parser.addErrorListener(&antlr4::QueryErrorHandler::INSTANCE);


  RawQueryListener rawListener(response);
  QueryNormListener normListener(
      dictSet, regex_index, response->request().max_regexword_matches(),
      normQueries, response->mutable_request()->phrase_length_min(),
      response->mutable_request()->phrase_length_max());
  QueryLengthVerifier lengthVerifier;
  ProxyListener listener;
  QueryComplexityMeasureListener complexity(
      response->request().max_regexword_matches(), &listener);

  listener.addListener(&complexity, true);
  listener.addListener(&lengthVerifier, false);
  listener.addListener(&normListener, false);
  listener.addListener(&rawListener, false);


  antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parser.query());
}

const std::set<std::string> lookup_word(const std::string &word,
                                        const Dictionaries::Map &dict) {
  std::set<std::string> words = { word };
  const auto range = dict.equal_range(word);
  for (auto it = range.first; it != range.second; ++it) {
    words.insert(it->second);
  }
  return words;
}


const std::vector<std::string> extract_longest_substrings(
    const generated::Query &query) {
  aitools::check(is_normalized(query), "is not normalized", query);

  size_t max_word_count = 1;
  std::vector<std::string> substrings;
  std::vector<std::string> word_buffer;
  for (const auto &unit : query.unit()) {
    switch (unit.tag()) {
      case generated::Query::Unit::WORD:
      case generated::Query::Unit::WORD_IN_DICTSET:
      case generated::Query::Unit::WORD_FOR_REGEX:
      case generated::Query::Unit::WORD_IN_ORDERSET:
      case generated::Query::Unit::WORD_IN_OPTIONSET:
      case generated::Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET:
      case generated::Query::Unit::WORD_FOR_REGEX_IN_ORDERSET:
        if (!unit.text().empty()) {
          word_buffer.push_back(unit.text());
        }
        break;
      default:
        if (word_buffer.size() >= max_word_count) {
          const auto substring = boost::join(word_buffer, " ");
          if (word_buffer.size() > max_word_count) {
            max_word_count = word_buffer.size();
            substrings.clear();
          }
          substrings.push_back(substring);
        }
        word_buffer.clear();
    }
  }
  if (word_buffer.size() >= max_word_count) {
    const auto substring = boost::join(word_buffer, " ");
    if (word_buffer.size() > max_word_count) {
      substrings.clear();
    }
    substrings.push_back(substring);
  }
  return substrings;
}


} // namespace netspeak
