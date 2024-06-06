#ifndef NETSPEAK_SERVICE_REQUEST_LOGGER_HPP
#define NETSPEAK_SERVICE_REQUEST_LOGGER_HPP


#include <netspeak/NetspeakService.grpc.pb.h>
#include <netspeak/NetspeakService.pb.h>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>

#include "netspeak/Netspeak.hpp"
#include "netspeak/util/lockable.hpp"


namespace netspeak::service {

class RequestLogger final : public NetspeakService::Service {
private:
  std::unique_ptr<NetspeakService::Service> service_;
  std::atomic<uint64_t> req_counter_;

  util::Lockable<std::ofstream> f_search_req_;
  util::Lockable<std::ofstream> f_search_error;

  util::Lockable<std::ofstream> f_get_corpora_req_;
  util::Lockable<std::ofstream> f_get_corpora_error;

public:
  RequestLogger() = delete;
  RequestLogger(const RequestLogger&) = delete;
  RequestLogger(std::unique_ptr<NetspeakService::Service> service, std::filesystem::path log_dir);
  ~RequestLogger() override {}

  grpc::Status Search(grpc::ServerContext* context, const SearchRequest* request, SearchResponse* response) override;
  grpc::Status GetCorpora(grpc::ServerContext* context, const CorporaRequest* request,
                          CorporaResponse* response) override;
};

} // namespace netspeak::service


#endif
