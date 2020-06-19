#ifndef NETSPEAK_INTERNAL_WORDS_HPP
#define NETSPEAK_INTERNAL_WORDS_HPP

#include <string>
#include <vector>

namespace netspeak {
namespace internal {

typedef std::string Word;

struct Words {
private:
  std::vector<Word> data_;

public:
  Words();

  std::vector<Word>& data() {
    return data_;
  }
  const std::vector<Word>& data() const {
    return data_;
  }

  bool empty() const {
    return data_.empty();
  }
  size_t size() const {
    return data_.size();
  }
  std::vector<Word>::const_iterator begin() const {
    return data_.begin();
  }
  std::vector<Word>::const_iterator end() const {
    return data_.end();
  }

  void push_back(const Word& word) {
    data_.push_back(word);
  }

  bool is_subsequence_of(const Words& words) const;
  bool is_subsequence_of(const std::vector<Word>& words) const;
};

std::ostream& operator<<(std::ostream& out, const Words& phrase);


} // namespace internal
} // namespace netspeak


#endif
