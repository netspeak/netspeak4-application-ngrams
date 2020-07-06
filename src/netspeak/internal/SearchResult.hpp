#ifndef NETSPEAK_INTERNAL_SEARCH_RESULT_HPP
#define NETSPEAK_INTERNAL_SEARCH_RESULT_HPP

#include <memory>
#include <vector>

#include "netspeak/internal/NormQuery.hpp"
#include "netspeak/internal/Phrase.hpp"


namespace netspeak {
namespace internal {


class SearchResult {
public:
  struct Item {
    std::shared_ptr<const NormQuery> query;
    Phrase phrase;

    Item() = delete;
    Item(const std::shared_ptr<const NormQuery>& query, const Phrase& phrase)
        : query(query), phrase(phrase) {}

    inline bool operator==(const Item& rhs) const {
      return phrase == rhs.phrase;
    }
    inline bool operator!=(const Item& rhs) const {
      return phrase != rhs.phrase;
    }
    inline bool operator<(const Item& rhs) const {
      return phrase < rhs.phrase;
    }
    inline bool operator<=(const Item& rhs) const {
      return phrase <= rhs.phrase;
    }
    inline bool operator>(const Item& rhs) const {
      return phrase > rhs.phrase;
    }
    inline bool operator>=(const Item& rhs) const {
      return phrase >= rhs.phrase;
    }
  };

private:
  std::vector<Item> phrases_;
  std::vector<const std::string> unknown_words_;

public:
  SearchResult(const SearchResult&) = delete;

  std::vector<Item>& phrases() {
    return phrases_;
  }
  const std::vector<Item>& phrases() const {
    return phrases_;
  }

  std::vector<const std::string>& unknown_words() {
    return unknown_words_;
  }
  const std::vector<const std::string>& unknown_words() const {
    return unknown_words_;
  }
};


} // namespace internal
} // namespace netspeak


#endif
