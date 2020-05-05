#include "RawQueryListener.hpp"

using namespace antlr4;

RawQueryListener::RawQueryListener(
    std::shared_ptr<netspeak::generated::RawResponse> response) {
  this->response = response;
}


void RawQueryListener::exitUnit(QueryParser::UnitContext *) {
  response->mutable_query()->add_unit()->CopyFrom(unit);
  unit.clear_tag();
  unit.clear_text();
}

void RawQueryListener::enterOptionset(QueryParser::OptionsetContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::OPTIONSET);
  buffer.append(std::string(1, netspeak::wildcard::lbracket));
  buffer.append(" ");
  response->add_query_token(std::string(1, netspeak::wildcard::lbracket));
}

void RawQueryListener::exitOptionset(QueryParser::OptionsetContext *) {
  buffer.append(std::string(1, netspeak::wildcard::rbracket));
  unit.set_text(buffer);
  buffer.clear();
  response->add_query_token(std::string(1, netspeak::wildcard::rbracket));
}

void RawQueryListener::enterOrderset(QueryParser::OrdersetContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::ORDERSET);
  buffer.append(std::string(1, netspeak::wildcard::lbrace));
  buffer.append(" ");
  response->add_query_token(std::string(1, netspeak::wildcard::lbrace));
}

void RawQueryListener::exitOrderset(QueryParser::OrdersetContext *) {
  buffer.append(std::string(1, netspeak::wildcard::rbrace));
  unit.set_text(buffer);
  buffer.clear();
  response->add_query_token(std::string(1, netspeak::wildcard::rbrace));
}

void RawQueryListener::enterDictset(QueryParser::DictsetContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::DICTSET);
  buffer.append(std::string(1, netspeak::wildcard::hash));
  buffer.append(" ");
  response->add_query_token(std::string(1, netspeak::wildcard::hash));
}

void RawQueryListener::exitDictset(QueryParser::DictsetContext *) {
  unit.set_text(buffer.substr(0, buffer.length() - 1));
  buffer.clear();
}

void RawQueryListener::enterPhrase(QueryParser::PhraseContext *) {
  buffer.append("\" ");
  response->add_query_token(std::string(1, netspeak::wildcard::dblquote));
}

void RawQueryListener::exitPhrase(QueryParser::PhraseContext *) {
  buffer.append("\" ");
  response->add_query_token(std::string(1, netspeak::wildcard::dblquote));
}

void RawQueryListener::enterRegexword(QueryParser::RegexwordContext *ctx) {
  if (!unit.has_tag()) {
    unit.set_tag(netspeak::generated::Query::Unit::REGEXWORD);
    unit.set_text(ctx->getText());
  } else {
    buffer.append(ctx->getText());
    buffer.append(" ");
  }
  response->add_query_token(ctx->getText());
}

void RawQueryListener::enterWord(QueryParser::WordContext *ctx) {
  if (!unit.has_tag()) {
    unit.set_tag(netspeak::generated::Query::Unit::WORD);
    unit.set_text(ctx->getText());
  } else {
    buffer.append(ctx->getText());
    buffer.append(" ");
  }
  response->add_query_token(ctx->getText());
}

void RawQueryListener::enterQmark(QueryParser::QmarkContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::QMARK);
  unit.set_text(std::string(1, netspeak::wildcard::qmark));
  response->add_query_token(std::string(1, netspeak::wildcard::qmark));
}


void RawQueryListener::enterPlus(QueryParser::PlusContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::PLUS);
  unit.set_text(std::string(1, netspeak::wildcard::plus));
  response->add_query_token(std::string(1, netspeak::wildcard::plus));
}

void RawQueryListener::enterAsterisk(QueryParser::AsteriskContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::ASTERISK);
  unit.set_text(std::string(1, netspeak::wildcard::asterisk));
  response->add_query_token(std::string(1, netspeak::wildcard::asterisk));
}


void RawQueryListener::enterAssociation(QueryParser::AssociationContext *) {
  unit.set_tag(netspeak::generated::Query::Unit::ASSOCIATION);
  unit.set_text(std::string(1, netspeak::wildcard::split));
  response->mutable_query()->add_unit()->CopyFrom(unit);
  response->add_query_token(std::string(1, netspeak::wildcard::split));
}
