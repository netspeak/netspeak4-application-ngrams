#include "netspeak/internal/Phrase.hpp"


namespace netspeak {
namespace internal {

const Phrase::Id::Local Phrase::Id::LOCAL_MAX =
    0xFFFFFFFFFFFFFF; // that's (1UL << 56) - 1
const Phrase::Id::Length Phrase::Id::LENGTH_MAX = 0xFF;

} // namespace internal
} // namespace netspeak
