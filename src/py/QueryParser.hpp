#include <memory>
#include <vector>

#include "netspeak/model/NormQuery.hpp"

namespace py {

using NormQueryUnitKind = netspeak::model::NormQuery::Unit::Tag;
using QueryUnitKind = netspeak::model::Query::Unit::Tag;

class NormQueryUnit {
private:
  NormQueryUnitKind kind;
  std::string text;
  QueryUnitKind source_kind;

public:
  NormQueryUnit() = delete;
  NormQueryUnit(NormQueryUnitKind kind, std::string text,
                QueryUnitKind source_kind);

  NormQueryUnitKind get_kind() const;
  const std::string& get_text() const;
  QueryUnitKind get_source_kind() const;
};

class NormQuery {
private:
  std::vector<NormQueryUnit> units;

public:
  std::vector<NormQueryUnit>& get_units();
};

class QueryParserOptions {
private:
  /**
   * @brief The maximum number of norm queries allowed to be returned.
   */
  uint32_t max_norm_queries;

  /**
   * @brief Same as \c max_length but as a minimum.
   */
  uint32_t min_length;
  /**
   * @brief The maximum length norm queries are allowed to have.
   *
   * This means that only phrases up to this length can be matched by the norm
   * queries.
   */
  uint32_t max_length;

  /**
   * @brief The maximum number of words each regex can be replaced with.
   *
   * A regex will (usually) be replaced with an alternation of words that
   * match that regex. This value determines with how many words each regex
   * will be replaced with at most.
   */
  uint32_t max_regex_matches;
  /**
   * @brief The maximum amount of time spend on each regex to find matching
   * words for that regex.
   */
  uint64_t max_regex_time_ns;

public:
  QueryParserOptions();

  uint32_t get_max_norm_queries() const;
  void set_max_norm_queries(uint32_t);
  uint32_t get_min_length() const;
  void set_min_length(uint32_t);
  uint32_t get_max_length() const;
  void set_max_length(uint32_t);
  uint32_t get_max_regex_matches() const;
  void set_max_regex_matches(uint32_t);
  uint64_t get_max_regex_time_ns() const;
  void set_max_regex_time_ns(uint64_t);
};


class QueryParser {
private:
  /* TODO: data */
public:
  QueryParser(const std::string& vocab, const std::string& dictionary_csv,
              bool lower_case);

  std::vector<NormQuery> parse(const std::string& query,
                               QueryParserOptions options) const;
};

} // namespace py
