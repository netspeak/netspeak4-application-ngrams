#include "cli/ServeCommand.hpp"

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <string>

#include "boost/filesystem.hpp"

#include "netspeak/Netspeak.hpp"
#include "netspeak/error.hpp"
#include "netspeak/service/UniqueMap.hpp"
#include "netspeak/util/PropertiesFormat.hpp"
#include "netspeak/util/service.hpp"


namespace cli {

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
using namespace netspeak;

std::string ServeCommand::desc() {
  return "Create a new gRPC server for a Netspeak index.";
};

void ServeCommand::add_options(
    boost::program_options::options_description_easy_init& easy_init) {
  easy_init("config,c", bpo::value<std::string>()->required(),
            "The configuration file of the index.");
  easy_init("port,p", bpo::value<uint16_t>()->required(),
            "The port on which the server will listen.");
}

service::UniqueMap::entry load_map_entry(const std::string& config_file) {
  Configuration config(config_file);

  service::Corpus corpus;
  corpus.set_key(config.get(Configuration::corpus_key));
  corpus.set_name(config.get(Configuration::corpus_name));
  corpus.set_language(config.get(Configuration::corpus_language));

  auto netspeak = std::make_unique<Netspeak>();
  netspeak->initialize(config);

  std::cout << "Loaded index " << corpus << "\n";

  return service::UniqueMap::entry{
    .instance = std::move(netspeak),
    .corpus = corpus,
  };
}

/**
 * @brief Returns the address of the localhost with the given port.
 */
std::string localhost(uint16_t port) {
  std::string local("localhost:");
  local.append(std::to_string(port));
  return local;
}

int ServeCommand::run(boost::program_options::variables_map variables) {
  auto port = variables["port"].as<uint16_t>();

  auto config_file = variables["config"].as<std::string>();
  std::vector<service::UniqueMap::entry> entries;
  entries.push_back(load_map_entry(config_file));
  service::UniqueMap service(std::move(entries));

  grpc::ServerBuilder builder;
  builder.AddListeningPort(localhost(port), grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port " << port << "\n";
  server->Wait();

  return EXIT_SUCCESS;
}

} // namespace cli
