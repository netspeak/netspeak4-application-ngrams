#include "cli/util.hpp"

#include <iomanip>

#include "netspeak/util/service.hpp"

namespace cli {

using namespace netspeak;


std::string localhost(uint16_t port) {
  std::string local("localhost:");
  local.append(std::to_string(port));
  return local;
}

service::CorporaResponse getCorpora(service::NetspeakService::Stub& stub,
                                    const std::string& stub_address) {
  service::CorporaRequest corporaReq;
  service::CorporaResponse corporaRes;
  grpc::ClientContext context;
  auto corpora_status = stub.GetCorpora(&context, corporaReq, &corporaRes);

  if (!corpora_status.ok()) {
    std::stringstream what;
    what << "Couldn't get corpora";
    if (!stub_address.empty()) {
      what << " from " << stub_address;
    }
    what << "\n" << corpora_status;
    throw std::logic_error(what.str());
  }

  return corporaRes;
}

} // namespace cli
