#include "netspeak/QueryNormalizer.hpp"

namespace netspeak {

QueryNormalizer::QueryNormalizer(const InitConfig& config)
    : regex_index_(config.regex_index) {}

struct NormQueryClass {
  size_t length;
  std::vector<const NormQuery> norm_queries;

  NormQueryClass() = delete;
  NormQueryClass(const NormQueryClass&) = delete;
  NormQueryClass(size_t length) : length(length) {}

  void push_back(const NormQuery& query) {
    if (query.size() != length) {
      throw std::logic_error(
          "All queries of a (length-) class have to have the same size.");
    }
    norm_queries.push_back(query);
  }


};


struct NormQueryCollection {
  std::vector<std::unique_ptr<NormQueryClass>> norm_queries;

  NormQueryCollection(const NormQueryCollection&) = delete;

  bool empty() const {
    return norm_queries.empty();
  }
  size_t size() const {
    return norm_queries.size();
  }

  void concat(std::unique_ptr<NormQueryCollection> other) {
    if (norm_queries.empty()) {
      // concat(EMPTY, other) == EMPTY
      return;
    }
    if (other->empty()) {
      // concat(this, EMPTY) == EMTPY
      norm_queries.clear();
      return;
    }

    // TODO
  }

  void alternation(std::unique_ptr<NormQueryCollection> other) {
    if (norm_queries.empty()) {
      // union(EMPTY, other) == other
      std::swap(norm_queries, other->norm_queries);
      return;
    }
    if (other->empty()) {
      // union(this, EMPTY) == this
      return;
    }

    // TODO
  }
};

void QueryNormalizer::normalize(const std::shared_ptr<const Query>& query,
                                const Options& options,
                                std::vector<NormQuery>& norm_queries) {
  // TODO:
  return;
}

} // namespace netspeak
