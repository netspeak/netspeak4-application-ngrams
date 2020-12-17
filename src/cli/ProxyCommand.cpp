#include "cli/ProxyCommand.hpp"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "cli/util.hpp"

#include "netspeak/Netspeak.hpp"
#include "netspeak/service/LoadBalanceProxy.hpp"
#include "netspeak/service/NetspeakService.grpc.pb.h"
#include "netspeak/util/service.hpp"

namespace cli {

namespace bpo = boost::program_options;
using namespace netspeak;

std::string ProxyCommand::desc() {
  return "A proxy to both combine and balance multiple Netspeak gRPC services.";
};

void ProxyCommand::add_options(bpo::options_description_easy_init& easy_init) {
  easy_init("port,p", bpo::value<uint16_t>()->required(),
            "The port on which the server will listen.");
  easy_init("source,s",
            bpo::value<std::vector<std::string>>()->required()->multitoken(),
            "The addresses of Netspeak gRPC servers.\n"
            "\n"
            "Example:\n"
            "    netspeak4 proxy -p 1234 -s localhost:1401 -s localhost:1402\n"
            "\n"
            "All the services behind the addresses are expected to be "
            "unchaning (the available corpora don't change) and to out-live "
            "this proxy. This cannot be used as a dynamic load balancer.");
}

typedef std::shared_ptr<service::NetspeakService::Stub> StubPtr;
StubPtr newStub(const std::string& address) {
  auto channel =
      grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
  return service::NetspeakService::NewStub(channel);
}

std::vector<std::pair<service::Corpus, StubPtr>> scanSources(
    const std::vector<std::string>& sources) {
  std::vector<std::pair<service::Corpus, StubPtr>> list;
  std::unordered_map<std::string, std::pair<service::Corpus, std::string>>
      corpora_map;
  for (const auto& address : sources) {
    std::cout << "New stub " << address << "\n";
    const auto stub = newStub(address);
    const auto corpora_res = getCorpora(*stub, address);
    std::cout << "Returned " << corpora_res.corpora().size() << " corpora\n";

    for (const auto& corpus : corpora_res.corpora()) {
      const auto& key = corpus.key();

      // check coprus
      const auto corpora_it = corpora_map.find(key);
      if (corpora_it == corpora_map.end()) {
        corpora_map.emplace(
            key, std::pair<service::Corpus, std::string>(corpus, address));
      } else {
        const auto& other_corpus = corpora_it->second.first;
        const auto& other_address = corpora_it->second.second;
        if (!service::LoadBalanceProxy::areCompatible(corpus, other_corpus)) {
          std::stringstream what;
          what << "Corpus " << corpus << " from " << address << " and corpus "
               << other_corpus << " from " << other_address
               << " are not compatible.\n";
          throw std::logic_error(what.str());
        }
      }

      std::cout << "    " << corpus << "\n";
      list.push_back(std::pair<service::Corpus, StubPtr>(corpus, stub));
    }
  }

  return list;
}

int ProxyCommand::run(bpo::variables_map variables) {
  const auto& sources = variables["source"].as<std::vector<std::string>>();

  const auto list = scanSources(sources);
  service::LoadBalanceProxy service(list);

  grpc::ServerBuilder builder;
  auto port = variables["port"].as<uint16_t>();
  builder.AddListeningPort("127.0.0.1:" + std::to_string(port),
                           grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port " << port << "\n";
  server->Wait();

  return EXIT_SUCCESS;
}

} // namespace cli
