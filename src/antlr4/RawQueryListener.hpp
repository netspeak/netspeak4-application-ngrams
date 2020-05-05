#ifndef RAWQUERYLISTENER_HPP
#define RAWQUERYLISTENER_HPP

#include <string>

#include "antlr4/generated/QueryBaseListener.h"

#include "netspeak/error.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/query_methods.hpp"

namespace antlr4 {

class RawQueryListener : public QueryBaseListener {
private:
  netspeak::generated::Query query;
  netspeak::generated::Query::Unit unit;
  std::string buffer;
  std::shared_ptr<netspeak::generated::RawResponse> response;

public:
  RawQueryListener(std::shared_ptr<netspeak::generated::RawResponse> response);

  void exitUnit(QueryParser::UnitContext *) override;
  void enterOptionset(QueryParser::OptionsetContext *) override;
  void exitOptionset(QueryParser::OptionsetContext *) override;


  void enterOrderset(QueryParser::OrdersetContext *) override;
  void exitOrderset(QueryParser::OrdersetContext *) override;

  void enterDictset(QueryParser::DictsetContext *) override;
  void exitDictset(QueryParser::DictsetContext *) override;
  void enterPhrase(QueryParser::PhraseContext *) override;
  void exitPhrase(QueryParser::PhraseContext *) override;
  void enterWord(QueryParser::WordContext *ctx) override;
  void enterQmark(QueryParser::QmarkContext *) override;
  void enterPlus(QueryParser::PlusContext *) override;
  void enterAsterisk(QueryParser::AsteriskContext *) override;
  void enterRegexword(QueryParser::RegexwordContext *ctx) override;
  void enterAssociation(QueryParser::AssociationContext *ctx) override;
};

} // namespace antlr4

#endif // RAWQUERYLISTENER_HPP
