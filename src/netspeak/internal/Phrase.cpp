#include "netspeak/internal/Phrase.hpp"


namespace netspeak {
namespace internal {


std::ostream& operator<<(std::ostream& out, const Phrase::Id& id) {
  return out << "Id(" << id.length() << ", " << id.local() << ")";
}
std::ostream& operator<<(std::ostream& out, const Phrase& phrase) {
  return out << "Phrase{ id: " << phrase.id() << ", freq: " << phrase.freq()
             << ", words: (" << phrase.words() << ") }";
}


} // namespace internal
} // namespace netspeak
