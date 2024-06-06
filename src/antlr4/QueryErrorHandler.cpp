#include <antlr4/QueryErrorHandler.hpp>
#include <format>
#include <netspeak/error.hpp>

namespace antlr4 {

void QueryErrorHandler::syntaxError(Recognizer*, Token* offendingSymbol, size_t line, size_t charPositionInLine,
                                    const std::string& msg, std::exception_ptr) {
  size_t startLine = line;
  size_t startPoint = (charPositionInLine + 1);
  size_t endLine = offendingSymbol->getLine();
  size_t endPoint = offendingSymbol->getStopIndex() + 1;

  std::string what = std::format("({}:{},{}:{}) {}", startLine, startPoint, endLine, endPoint, msg);
  throw netspeak::invalid_query_error(what);
}

} // namespace antlr4
