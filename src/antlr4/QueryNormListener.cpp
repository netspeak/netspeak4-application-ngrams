#include "QueryNormListener.hpp"

#include "netspeak/regex/parsers.hpp"

using namespace antlr4;

QueryNormListener::QueryNormListener(
    netspeak::Dictionaries::Map *dict, netspeak::regex::RegexIndex *regex_index,
    uint32_t maxRegexMatches,
    std::vector<netspeak::generated::Query> *normQueries,
    size_t min_phrase_length, size_t max_phrase_length)
    : regex_match_cache() {
  inOptionSet = false;
  inOrderSet = false;
  inDictSet = false;
  inPhrase = false;
  this->maxRegexMatches = maxRegexMatches;
  this->dict = dict;
  this->regex_index = regex_index;
  this->normQueries = normQueries;
  this->min_phrase_length = min_phrase_length;
  this->max_phrase_length = max_phrase_length;
}


void QueryNormListener::enterUnits(QueryParser::UnitsContext *) {
  partlyNormQueries.push_back(netspeak::generated::Query());
}

void QueryNormListener::exitUnits(QueryParser::UnitsContext *) {
  size_t size;
  for (std::vector<netspeak::generated::Query>::iterator norm =
           partlyNormQueries.begin();
       norm != partlyNormQueries.end(); norm++) {
    size = norm->unit_size();
    if (this->min_phrase_length <= size && size <= this->max_phrase_length) {
      normQueries->push_back(*norm);
    }
  }
  partlyNormQueries.clear();
}

void QueryNormListener::enterUnit(QueryParser::UnitContext *) {
  tempQueries.swap(partlyNormQueries);
}


void QueryNormListener::exitUnit(QueryParser::UnitContext *) {
  tempQueries.clear();
}

void QueryNormListener::enterOptionset(QueryParser::OptionsetContext *) {
  inOptionSet = true;
}

void QueryNormListener::exitOptionset(QueryParser::OptionsetContext *ctx) {
  inOptionSet = false;

  if (ctx->children.size() <= 3)
    partlyNormQueries.insert(partlyNormQueries.end(), tempQueries.begin(),
                             tempQueries.end());
  phraseUnit.clear();
  tempQueries.clear();
}

void QueryNormListener::enterOrderset(QueryParser::OrdersetContext *) {
  inOrderSet = true;
}

void QueryNormListener::exitOrderset(QueryParser::OrdersetContext *) {
  inOrderSet = false;

  std::sort(permutation.begin(), permutation.end(), compareVector());

  do {
    for (const auto &temp_q : tempQueries) {
      netspeak::generated::Query query(temp_q);
      std::vector<netspeak::generated::Query> expandQuery;
      expandQuery.push_back(query);
      std::vector<netspeak::generated::Query> temp;

      for (std::vector<std::vector<netspeak::generated::Query::Unit>>::iterator
               permut = permutation.begin();
           permut != permutation.end(); permut++) {
        for (std::vector<netspeak::generated::Query::Unit>::iterator unit =
                 permut->begin();
             unit != permut->end(); unit++) {
          if (unit->tag() == netspeak::generated::Query::Unit::REGEXWORD) {
            std::vector<std::string> matches = match_regex_query(unit->text());
            netspeak::generated::Query::Unit gunther;
            gunther.set_tag(
                netspeak::generated::Query::Unit::WORD_FOR_REGEX_IN_ORDERSET);

            // replace a regex with all found words (each in a new query object)
            for (std::vector<std::string>::iterator match = matches.begin();
                 match != matches.end(); match++) {
              for (std::vector<netspeak::generated::Query>::iterator
                       currentQuery = expandQuery.begin();
                   currentQuery != expandQuery.end(); currentQuery++) {
                gunther.set_text(*match);
                netspeak::generated::Query temporaer(*currentQuery);
                temporaer.add_unit()->CopyFrom(gunther);
                temp.push_back(temporaer);
              }
            }
            expandQuery.clear();
            expandQuery.swap(temp);

          } else {
            for (std::vector<netspeak::generated::Query>::iterator
                     currentQuery = expandQuery.begin();
                 currentQuery != expandQuery.end(); currentQuery++) {
              unit->set_tag(netspeak::generated::Query::Unit::WORD_IN_ORDERSET);
              currentQuery->add_unit()->CopyFrom(*unit);
            }
          }
        }
      }
      partlyNormQueries.insert(partlyNormQueries.end(), expandQuery.begin(),
                               expandQuery.end());
    }

  } while (std::next_permutation(permutation.begin(), permutation.end(),
                                 compareVector()));

  permutation.clear();
  phraseUnit.clear();
  tempQueries.clear();
}

void QueryNormListener::enterDictset(QueryParser::DictsetContext *) {
  inDictSet = true;
}
void QueryNormListener::exitDictset(QueryParser::DictsetContext *) {
  inDictSet = false;
}

void QueryNormListener::enterPhrase(QueryParser::PhraseContext *) {
  inPhrase = true;
}
void QueryNormListener::exitPhrase(QueryParser::PhraseContext *) {
  inPhrase = false;


  if (inOptionSet) {
    for (const auto &temp_q : tempQueries) {
      std::vector<netspeak::generated::Query> tempQuery;
      tempQuery.push_back(netspeak::generated::Query(temp_q));
      std::vector<netspeak::generated::Query> temp;

      for (auto &unit : phraseUnit) {
        if (unit.tag() == netspeak::generated::Query::Unit::REGEXWORD) {
          std::vector<std::string> matches = match_regex_query(unit.text());

          for (const auto &match : matches) {
            for (auto qr = tempQuery.begin(); qr != tempQuery.end(); qr++) {
              netspeak::generated::Query temporaer(*qr);
              netspeak::generated::Query::Unit tz;
              tz.set_tag(netspeak::generated::Query::Unit::
                             WORD_FOR_REGEX_IN_OPTIONSET);
              tz.set_text(match);
              temporaer.add_unit()->CopyFrom(tz);
              temp.push_back(temporaer);
            }
          }
          tempQuery.clear();
          tempQuery.swap(temp);
        } else {
          for (auto qr = tempQuery.begin(); qr != tempQuery.end(); qr++) {
            unit.set_tag(netspeak::generated::Query::Unit::WORD_IN_OPTIONSET);
            qr->add_unit()->CopyFrom(unit);
          }
        }
      }
      partlyNormQueries.insert(partlyNormQueries.end(), tempQuery.begin(),
                               tempQuery.end());
    }
  } else {
    if (!phraseUnit.empty())
      permutation.push_back(phraseUnit);
  }
  phraseUnit.clear();
}

void QueryNormListener::enterAsterisk(QueryParser::AsteriskContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::QMARK_FOR_ASTERISK);
  unit.set_text(std::string(1, netspeak::wildcard::qmark));

  for (const auto &temp_q : tempQueries) {
    for (int j = this->max_phrase_length; j >= temp_q.unit_size(); j--) {
      netspeak::generated::Query query(temp_q);
      for (int t = j; t < this->max_phrase_length; t++) {
        query.add_unit()->CopyFrom(unit);
      }
      partlyNormQueries.push_back(query);
    }
  }
}

void QueryNormListener::enterRegexword(QueryParser::RegexwordContext *ctx) {
  std::string regex_query = ctx->getText();
  std::vector<std::string> matches = match_regex_query(regex_query);

  if (inPhrase) {
    netspeak::generated::Query::Unit pUnit;
    pUnit.set_text(regex_query);
    pUnit.set_tag(netspeak::generated::Query::Unit::REGEXWORD);
    phraseUnit.push_back(pUnit);

  } else if (inOptionSet) {
    unit.set_tag(netspeak::generated::Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET);
    for (const auto &regexword : matches) {
      for (const auto &temp_q : tempQueries) {
        netspeak::generated::Query query(temp_q);
        unit.set_text(regexword);
        query.add_unit()->CopyFrom(unit);
        partlyNormQueries.push_back(query);
      }
    }
  } else if (inOrderSet) {
    std::vector<netspeak::generated::Query::Unit> pUnit;
    netspeak::generated::Query::Unit unit;
    unit.set_tag(netspeak::generated::Query::Unit::REGEXWORD);
    unit.set_text(regex_query);
    pUnit.push_back(unit);
    permutation.push_back(pUnit);

  } else {
    unit.set_tag(netspeak::generated::Query::Unit::WORD_FOR_REGEX);

    for (const auto &regexword : matches) {
      unit.set_text(regexword);
      for (const auto &temp_q : tempQueries) {
        netspeak::generated::Query query(temp_q);
        query.add_unit()->CopyFrom(unit);
        partlyNormQueries.push_back(query);
      }
    }
  }
}

void QueryNormListener::enterWord(QueryParser::WordContext *ctx) {
  std::string text = ctx->getText();
  std::string word;

  // unescape all characters
  for (std::string::iterator character = text.begin(); character != text.end();
       character++) {
    if (*character != '\\') {
      word.push_back(*character);
    } else {
      character++;
      word.push_back(*character);
    }
  }
  unit.set_text(word);

  if (inPhrase) {
    netspeak::generated::Query::Unit pUnit;
    pUnit.set_text(word);
    phraseUnit.push_back(pUnit);

  } else if (inOptionSet) {
    for (const auto &temp_q : tempQueries) {
      netspeak::generated::Query query(temp_q);
      unit.set_tag(netspeak::generated::Query::Unit::WORD_IN_OPTIONSET);
      query.add_unit()->CopyFrom(unit);
      partlyNormQueries.push_back(query);
    }
  } else if (inOrderSet) {
    std::vector<netspeak::generated::Query::Unit> pUnit;
    netspeak::generated::Query::Unit unit;
    unit.set_tag(netspeak::generated::Query::Unit::WORD_IN_ORDERSET);
    unit.set_text(ctx->getText());
    pUnit.push_back(unit);
    permutation.push_back(pUnit);

  } else if (inDictSet) {
    std::set<std::string> syn = netspeak::lookup_word(ctx->getText(), *dict);
    std::vector<std::string> container;

    unit.set_tag(netspeak::generated::Query::Unit::WORD_IN_DICTSET);
    for (std::set<std::string>::iterator wort = syn.begin(); wort != syn.end();
         wort++) {
      size_t i = 0;
      size_t end;
      while ((end = wort->find(' ', i)) != std::string::npos) {
        container.push_back(wort->substr(i, end - i));
        i = end + 1;
      }

      container.push_back(wort->substr(i, wort->length()));

      for (const auto &temp_q : tempQueries) {
        netspeak::generated::Query query(temp_q);

        for (const auto &synonym : container) {
          unit.set_text(synonym);
          query.add_unit()->CopyFrom(unit);
        }
        partlyNormQueries.push_back(query);
      }
      container.clear();
    }
  } else {
    unit.set_tag(netspeak::generated::Query::Unit::WORD);
    for (auto &temp_q : tempQueries) {
      temp_q.add_unit()->CopyFrom(unit);
      partlyNormQueries.push_back(temp_q);
    }
  }
}

void QueryNormListener::enterQmark(QueryParser::QmarkContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::QMARK);
  unit.set_text(std::string(1, netspeak::wildcard::qmark));

  for (auto &temp_q : tempQueries) {
    temp_q.add_unit()->CopyFrom(unit);
    partlyNormQueries.push_back(temp_q);
  }
}


void QueryNormListener::enterPlus(QueryParser::PlusContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::QMARK_FOR_PLUS);
  unit.set_text(std::string(1, netspeak::wildcard::qmark));


  for (auto &temp_q : tempQueries) {
    temp_q.add_unit()->CopyFrom(unit);
    for (int j = this->max_phrase_length; j >= temp_q.unit_size(); j--) {
      netspeak::generated::Query query(temp_q);
      for (int t = j; t < this->max_phrase_length; t++) {
        query.add_unit()->CopyFrom(unit);
      }
      partlyNormQueries.push_back(query);
    }
  }
}

std::vector<std::string> QueryNormListener::match_regex_query(
    std::string regex_query) {
  if (!regex_index) {
    // no regex index, no matches
    return std::vector<std::string>();
  }

  auto temp = regex_match_cache.find(regex_query);
  if (temp != regex_match_cache.end()) {
    // cache hit
    return temp->second;
  }

  // query the index and cache the result
  auto query = netspeak::regex::parse_netspeak_regex_query(regex_query);
  std::vector<std::string> matches;
  regex_index->match_query(query, matches, maxRegexMatches,
                           MAX_REGEX_SEARCH_TIME);

  // the regex cache is only used within a query, so only one thread will every
  // touch the cache
  regex_match_cache.emplace(regex_query, matches);
  return matches;
}
