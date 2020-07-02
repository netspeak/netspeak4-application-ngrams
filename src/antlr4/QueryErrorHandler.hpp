#ifndef QUERYERRORHANDLER_HPP
#define QUERYERRORHANDLER_HPP

#include <string>

#include "antlr4-runtime.h"
#include "antlr4/generated/QueryLexer.h"

namespace antlr4 {


/**
 * @brief The QueryErrorHandler class
 *
 * Is called whenever a error in the parser occurs.
 */
class QueryErrorHandler : public antlr4::BaseErrorListener {
public:
  static QueryErrorHandler INSTANCE;

  /**
   * @brief syntaxError
   * @param offendingSymbol
   * @param line
   * @param charPositionInLine
   * @param msg
   *
   * Is called when a syntax error occurs e.g. an unexpected token is read.
   */
  virtual void syntaxError(Recognizer *, Token *offendingSymbol, size_t line,
                           size_t charPositionInLine, const std::string &msg,
                           std::exception_ptr) override;
};

} // namespace antlr4
#endif // QUERYERRORHANDLER_HPP
