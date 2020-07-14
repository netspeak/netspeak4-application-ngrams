#ifndef NETSPEAK_INDEXING_HPP
#define NETSPEAK_INDEXING_HPP


#include <memory>

#include "netspeak/Netspeak.hpp"
#include "netspeak/service/NetspeakService.grpc.pb.h"
#include "netspeak/service/NetspeakService.pb.h"


namespace netspeak {
namespace service {


/**
 * @brief An implementation of the Netspeak gRPC service that will forward all
 * requests to a single Netspeak instance.
 *
 */
class SingleInstance final : public service::NetspeakService::Service {
private:
  std::shared_ptr<Netspeak> instance_;
  Corpus corpus_;

public:
  SingleInstance() = delete;
  SingleInstance(const SingleInstance&) = delete;
  SingleInstance(const std::shared_ptr<Netspeak> instance, const Corpus& corpus)
      : instance_(instance), corpus_(corpus) {}
  ~SingleInstance() override {}

  grpc::Status Search(grpc::ServerContext* context,
                      const SearchRequest* request,
                      SearchResponse* response) override {
    // check the corpus
    if (request->corpus() != corpus_.key()) {
      auto error = response->mutable_error();
      error->set_kind(SearchResponse::Error::INVALID_CORPUS);
      error->set_message("Unknown corpus");
      return grpc::Status::OK;
    }

    // forward the request to our Netspeak instance
    // (search is guaranteed not to throw, so we don't need to do anything)
    instance_->search(*request, *response);
    return grpc::Status::OK;
  }
  grpc::Status GetCorpora(grpc::ServerContext* context,
                          const CorporaRequest* request,
                          CorporaResponse* response) override {
    // we only need to add our single corpus to the response
    response->add_corpora()->CopyFrom(corpus_);
    return grpc::Status::OK;
  }
};


} // namespace service
} // namespace netspeak


#endif
