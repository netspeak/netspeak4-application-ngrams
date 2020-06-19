#ifndef NETSPEAK_NETSPEAK_SERVICE_IMPL_HPP
#define NETSPEAK_NETSPEAK_SERVICE_IMPL_HPP


#include <grpc/grpc.h>
#include <grpcpp/server_context.h>

#include "NetspeakService.grpc.pb.h"

namespace netspeak {

using namespace grpc;

class NetspeakServiceImpl final : public service::NetspeakService::Service {
public:
  NetspeakServiceImpl() {}
  ~NetspeakServiceImpl() override{

  };
  Status Search(ServerContext* context, const service::SearchRequest* request,
                service::SearchResponse* response) override {
    return Status::OK;
  }
  Status GetCorpora(ServerContext* context,
                    const service::CorporaRequest* request,
                    service::CorporaResponse* response) override {
    return Status::OK;
  }
};


} // namespace netspeak


#endif
