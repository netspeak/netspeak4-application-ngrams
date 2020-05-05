#include "QueryComplexityMeasureListener.hpp"

using namespace antlr4;

QueryComplexityMeasureListener::QueryComplexityMeasureListener(
    int maxRegexMatches, ProxyListener *proxy) {
  this->proxy = proxy;
  this->maxRegexMatches = maxRegexMatches;
}

void QueryComplexityMeasureListener::exitQuery(QueryParser::QueryContext *ctx) {
  if (this->totalComplexity > MAX_WORST_CASE_COMPLEXITY)
    throwError(ctx, netspeak::query_error_message::too_complex(
                        this->totalComplexity, MAX_WORST_CASE_COMPLEXITY));
}

void QueryComplexityMeasureListener::enterUnits(QueryParser::UnitsContext *) {
  if (this->totalComplexity >= MAX_WORST_CASE_COMPLEXITY) {
    proxy->notValid();
  }
}

void QueryComplexityMeasureListener::exitUnits(QueryParser::UnitsContext *) {
  this->totalComplexity = safeAdd(this->totalComplexity, this->temp);
  temp = 1;
}


void QueryComplexityMeasureListener::exitUnit(QueryParser::UnitContext *) {
  if (this->temp >= MAX_WORST_CASE_COMPLEXITY) {
    proxy->notValid();
  }
}

void QueryComplexityMeasureListener::enterOptionset(
    QueryParser::OptionsetContext *ctx) {
  this->temp = safeMult(ctx->children.size() - 2, temp);
}

void QueryComplexityMeasureListener::enterOrderset(
    QueryParser::OrdersetContext *ctx) {
  this->temp = safeMult(faculty(ctx->children.size() - 2), temp);
}

void QueryComplexityMeasureListener::exitOrderset(
    QueryParser::OrdersetContext *) {
  if (this->temp >= MAX_WORST_CASE_COMPLEXITY) {
    proxy->notValid();
  }
}

void QueryComplexityMeasureListener::enterDictset(
    QueryParser::DictsetContext *) {
  this->temp = safeMult(temp, DICTSET_FACTOR);
}

void QueryComplexityMeasureListener::enterAsterisk(
    QueryParser::AsteriskContext *) {
  this->temp = safeMult(temp, ASTERISK_FACTOR);
}

void QueryComplexityMeasureListener::enterRegexword(
    QueryParser::RegexwordContext *) {
  this->temp = safeMult(temp, maxRegexMatches);
  this->temp = safeMult(temp, REGEXWORD_FACTOR);
  if (this->temp >= MAX_WORST_CASE_COMPLEXITY) {
    proxy->notValid();
  }
}

void QueryComplexityMeasureListener::enterWord(QueryParser::WordContext *) {
  this->temp = safeMult(temp, WORD_FACTOR);
}

void QueryComplexityMeasureListener::enterQmark(QueryParser::QmarkContext *) {
  this->temp = safeMult(temp, QMARK_FACTOR);
}

void QueryComplexityMeasureListener::enterPlus(QueryParser::PlusContext *) {
  this->temp = safeMult(temp, PLUS_FACTOR);
}
