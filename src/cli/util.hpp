#ifndef CLI_UTIL_HPP
#define CLI_UTIL_HPP

#include <string>

#include "netspeak/service/NetspeakService.grpc.pb.h"

namespace cli {

/**
 * @brief Returns the address of the localhost with the given port.
 */
std::string localhost(uint16_t port);

/**
 * @brief Returns the corpora of the given stub.
 *
 * @param stub
 * @param stub_address The address of the stub or the empty string.
 */
netspeak::service::CorporaResponse getCorpora(
    netspeak::service::NetspeakService::Stub& stub,
    const std::string& stub_address = "");

} // namespace cli

#endif
