#include <netspeak/model/Phrase.hpp>


namespace netspeak::model {


std::ostream& operator<<(std::ostream& out, const Phrase::Id& id) {
  return out << "Id(" << id.length() << ", " << id.local() << ")";
}
std::ostream& operator<<(std::ostream& out, const Phrase& phrase) {
  return out << "Phrase{ id: " << phrase.id() << ", freq: " << phrase.freq() << ", words: (" << phrase.words() << ") }";
}


} // namespace netspeak::model
