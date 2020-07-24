#include "cli/ServeCommand.hpp"

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <string>

#include "boost/filesystem.hpp"

#include "netspeak/Netspeak.hpp"
#include "netspeak/service/UniqueMap.hpp"


namespace cli {

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
using namespace netspeak;

void ServeCommand::add_options(
    boost::program_options::options_description_easy_init& easy_init) {
  easy_init("config,c", bpo::value<std::string>()->required(),
            "The configuration file of the index");
  easy_init("port,p", bpo::value<uint16_t>()->required(),
            "The port on which the server will listen");
}

service::UniqueMap::entry load_config(const std::string& config) {
  // TODO:
  throw std::logic_error("Not implemented yet.");
}

int ServeCommand::run(boost::program_options::variables_map variables) {
  bpo::notify(variables);

  auto port = variables["port"].as<uint16_t>();
  std::string server_address("localhost:");
  server_address.append(std::to_string(port));

  auto config_file = variables["config"].as<std::string>();
  std::vector<service::UniqueMap::entry> entries;
  entries.push_back(std::move(load_config(config_file)));
  service::UniqueMap service(std::move(entries));

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

  return EXIT_SUCCESS;
}

} // namespace cli
