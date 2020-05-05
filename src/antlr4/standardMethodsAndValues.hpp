#ifndef STANDARDMETHODSANDVALUES
#define STANDARDMETHODSANDVALUES

#include <chrono>

#include "antlr4-runtime.h"

#include "netspeak/error.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"

// max length a phrase could have
const int MAX_PHRASE_LENGTH = 5;


// constants for the worst case approximation in QueryComplexityMeasureListener
const unsigned int MAX_WORST_CASE_COMPLEXITY = 10000;
const unsigned int WORD_FACTOR = 1;
const unsigned int REGEXWORD_FACTOR = 1;
const unsigned int QMARK_FACTOR = 1;
const unsigned int DICTSET_FACTOR = 5;
const unsigned int PLUS_FACTOR = 4;
const unsigned int ASTERISK_FACTOR = 5;
const unsigned int SPLIT_ADDEND = 0;

// time for searching for words matching the given regex
const std::chrono::milliseconds MAX_REGEX_SEARCH_TIME(30);


/**
 * \brief The compareVector struct
 * Be used to compare two vector object to use the permutation function
 * empty objects will terminate the application
 */
struct compareVector {
  bool operator()(
      const std::vector<netspeak::generated::Query::Unit>& a,
      const std::vector<netspeak::generated::Query::Unit>& b) const {
    netspeak::generated::Query::Unit c = a.front();
    netspeak::generated::Query::Unit d = b.front();

    return c.text() < d.text();
  }
};

/**
 * @brief throwError
 * @param ctx Object causing the error
 */
inline void throwError(antlr4::ParserRuleContext* ctx, std::string msg) {
  throw netspeak::invalid_query(
      ctx->getStart()->getLine(), ctx->getStart()->getStartIndex(),
      ctx->getStop()->getLine(), ctx->getStop()->getStopIndex(), msg);
}

/**
 * @brief safeAdd
 * @param a
 * @param b
 * @return the sum or if overflow UINT32_MAX
 *
 */
inline uint32_t safeAdd(uint32_t a, uint32_t b) {
  if (a + b < a)
    return UINT32_MAX;
  return a + b;
}

/**
 * @brief safeMult
 * @param a
 * @param b
 * @return the product or if overflow UINT32_MAX
 */
inline uint32_t safeMult(uint32_t a, uint32_t b) {
  uint64_t res = (uint64_t)a * (uint64_t)b;
  if (res >= UINT_MAX)
    return UINT_MAX;
  return (uint32_t)res;
}

/**
 * @brief faculty
 * @param n
 * @return faculty of n
 */
inline int faculty(int n) {
  int i = 1;
  while (n > 1) {
    i *= n;
    n--;
  }
  return i;
}

#endif // STANDARDMETHODSANDVALUES
