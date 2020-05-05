#include "QueryLengthVerifier.hpp"

using namespace antlr4;

QueryLengthVerifier::QueryLengthVerifier() {
  unitCount = 0;
  phraseCount = INT_MAX;
  counter = 0;
}

void QueryLengthVerifier::exitUnits(QueryParser::UnitsContext *ctx) {
  if (unitCount > MAX_PHRASE_LENGTH)
    throwError(ctx, netspeak::query_error_message::too_many_words);
  unitCount = 0;
}

void QueryLengthVerifier::exitUnit(QueryParser::UnitContext *) {
  unitCount += counter;
  phraseCount = INT_MAX;
  counter = 0;
}


void QueryLengthVerifier::exitOptionset(QueryParser::OptionsetContext *ctx) {
  if (ctx->children.size() <= 3) {
    counter = 0;
  } else if (ctx->word().empty() && ctx->regexword().empty()) {
    counter = phraseCount;
  } else {
    counter = 1;
  }
}

void QueryLengthVerifier::enterPhrase(QueryParser::PhraseContext *ctx) {
  phraseCount = (phraseCount > ctx->children.size() - 2)
                    ? ctx->children.size() - 2
                    : phraseCount;
}


void QueryLengthVerifier::enterRegexword(QueryParser::RegexwordContext *) {
  counter++;
}

void QueryLengthVerifier::enterWord(QueryParser::WordContext *) {
  counter++;
}

void QueryLengthVerifier::enterQmark(QueryParser::QmarkContext *) {
  counter++;
}

void QueryLengthVerifier::enterPlus(QueryParser::PlusContext *) {
  counter++;
}
