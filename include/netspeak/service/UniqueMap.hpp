#ifndef NETSPEAK_SERVICE_UNIQUE_MAP_HPP
#define NETSPEAK_SERVICE_UNIQUE_MAP_HPP


#include <netspeak/NetspeakService.grpc.pb.h>
#include <netspeak/NetspeakService.pb.h>

#include <memory>
#include <unordered_map>

#include "netspeak/Netspeak.hpp"


namespace netspeak::service {

/**
 * @brief An implementation of the Netspeak gRPC service that will forward all
 * requests to a unique Netspeak instance for the given corpus.
 *
 * Each corpus (key) can have at most one Netspeak instance.
 */
class UniqueMap final : public NetspeakService::Service {
private:
  std::unordered_map<std::string, std::unique_ptr<Netspeak>> instances_;
  std::vector<Corpus> corpora_;

public:
  struct entry {
    std::unique_ptr<Netspeak> instance;
    Corpus corpus;
  };

public:
  UniqueMap() = delete;
  UniqueMap(const UniqueMap&) = delete;
  explicit UniqueMap(std::unique_ptr<std::vector<entry>> entries_ptr);
  ~UniqueMap() override {}

  grpc::Status Search(grpc::ServerContext* context, const SearchRequest* request, SearchResponse* response) override {
    return Search_(context, request, response);
  }
  grpc::Status GetCorpora(grpc::ServerContext* context, const CorporaRequest* request,
                          CorporaResponse* response) override {
    return GetCorpora_(context, request, response);
  }

private:
  // These functions exists because we cannot declare the override as `const`.
  // The `const` means that the compiler will not allow us to modify the state
  // of the object and as an immutable object, it's inherently thread-safe.

  grpc::Status Search_(grpc::ServerContext* context, const SearchRequest* request, SearchResponse* response) const;
  grpc::Status GetCorpora_(grpc::ServerContext* context, const CorporaRequest* request,
                           CorporaResponse* response) const;
};

} // namespace netspeak::service


#endif
