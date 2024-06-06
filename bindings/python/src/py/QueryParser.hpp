#include <memory>
#include <vector>

#include "netspeak/QueryNormalizer.hpp"
#include "netspeak/model/NormQuery.hpp"

namespace py {

class NormQueryUnit {
public:
  typedef netspeak::model::NormQuery::Unit::Tag Kind;
  typedef netspeak::model::Query::Unit::Tag QueryUnitKind;

private:
  Kind kind_;
  std::string text_;
  QueryUnitKind source_kind_;

public:
  NormQueryUnit() = delete;
  NormQueryUnit(Kind kind, std::string text, QueryUnitKind source_kind);

  Kind get_kind() const;
  const std::string& get_text() const;
  QueryUnitKind get_source_kind() const;
};

class NormQuery {
private:
  std::vector<NormQueryUnit> units_;

public:
  std::vector<NormQueryUnit>& get_units();
};

class QueryParserOptions {
public:
  uint32_t max_norm_queries = 1000;

  uint32_t min_length = 1;
  uint32_t max_length = 5;

  uint32_t max_regex_matches = 100;
  uint32_t max_regex_time_ms = 20;
};


class QueryParser {
private:
  netspeak::QueryNormalizer normalizer;

public:
  QueryParser(const std::string& vocab, const std::string& dictionary_csv,
              bool lower_case);

  std::vector<NormQuery> parse(const std::string& query,
                               QueryParserOptions options);
};

} // namespace py
