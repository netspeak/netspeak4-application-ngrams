#ifndef NETSPEAK_INTERNAL_TYPEDEFS_HPP
#define NETSPEAK_INTERNAL_TYPEDEFS_HPP

#include "aitools/value/pair.hpp"

namespace netspeak {
namespace internal {

/**
 * @brief Each unit of a Netspeak query is given a unique id to identify it
 * throughout the evaluation of the query. This is the type of that unique id.
 */
typedef uint32_t QueryUnitId;

} // namespace internal
} // namespace netspeak

#endif
