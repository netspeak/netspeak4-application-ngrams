#ifndef NETSPEAK_INTERNAL_WORDS_HPP
#define NETSPEAK_INTERNAL_WORDS_HPP

#include <ostream>
#include <string>
#include <vector>

namespace netspeak {
namespace internal {

typedef std::string Word;

struct Words {
private:
  std::vector<const Word> data_;

public:
  Words();

  std::vector<const Word>& data() {
    return data_;
  }
  const std::vector<const Word>& data() const {
    return data_;
  }

  bool empty() const {
    return data_.empty();
  }
  size_t size() const {
    return data_.size();
  }
  std::vector<const Word>::const_iterator begin() const {
    return data_.begin();
  }
  std::vector<const Word>::const_iterator end() const {
    return data_.end();
  }

  const Word& operator[](size_t i) const {
    return data_[i];
  }

  void push_back(const Word& word) {
    data_.push_back(word);
  }

  bool is_subsequence_of(const Words& words) const;
  bool is_subsequence_of(const std::vector<const Word>& words) const;
};

std::ostream& operator<<(std::ostream& out, const Words& phrase);


} // namespace internal
} // namespace netspeak


#endif
