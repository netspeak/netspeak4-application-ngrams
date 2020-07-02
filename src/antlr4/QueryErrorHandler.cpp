#include "QueryErrorHandler.hpp"

#include "netspeak/error.hpp"


namespace antlr4 {


QueryErrorHandler QueryErrorHandler::INSTANCE;


void QueryErrorHandler::syntaxError(Recognizer *, Token *offendingSymbol,
                                    size_t line, size_t charPositionInLine,
                                    const std::string &msg,
                                    std::exception_ptr) {
  throw netspeak::invalid_query(line, charPositionInLine,
                                offendingSymbol->getLine(),
                                offendingSymbol->getStopIndex() + 1, msg);
}


} // namespace antlr4
