#ifndef NETSPEAK_UTIL_SERVICE_HPP
#define NETSPEAK_UTIL_SERVICE_HPP


#include <grpcpp/channel.h>
#include <netspeak/NetspeakService.pb.h>

#include <ostream>

namespace grpc {

std::ostream& operator<<(std::ostream& out, const grpc::Status& status);

}

namespace netspeak::service {

std::ostream& operator<<(std::ostream& out, const Corpus& corpus);

} // namespace netspeak::service


#endif
