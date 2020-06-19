#include "netspeak/QueryNormalizer.hpp"

namespace netspeak {

QueryNormalizer::QueryNormalizer(const InitConfig& config)
    : regex_index_(config.regex_index) {}


struct NormQueryCollection {
  std::vector<NormQuery> norm_queries;

  NormQueryCollection(const NormQueryCollection&) = delete;

  bool empty() const {
    return norm_queries.empty();
  }
  size_t size() const {
    return norm_queries.size();
  }

  void append(std::unique_ptr<NormQueryCollection> other) {
    if (norm_queries.empty()) {
      // concat(EMPTY, other) == EMPTY
      return;
    }
    if (other->empty()) {
      // concat(this, EMPTY) == EMTPY
      norm_queries.clear();
      return;
    }

    if (other->size() == 1) {
      // no need to make a copy. Just append the one query to all
      const auto& other_units = other->norm_queries[0].units();

      for (auto& q : norm_queries) {
        auto& q_units = q.units();
        q_units.insert(q_units.end(), other_units.begin(), other_units.end());
      }
    } else {
      // TODO: implement
    }
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
