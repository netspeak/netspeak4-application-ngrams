#include "cli/ServeCommand.hpp"

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <string>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/optional.hpp"

#include "cli/util.hpp"

#include "netspeak/Netspeak.hpp"
#include "netspeak/error.hpp"
#include "netspeak/service/UniqueMap.hpp"
#include "netspeak/util/PropertiesFormat.hpp"
#include "netspeak/util/glob.hpp"
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
  easy_init("config,c",
            bpo::value<std::vector<std::string>>()->required()->multitoken(),
            "The configuration file(s) of the index(es).\n"
            "\n"
            "It's possible to give multiple config files like this:\n"
            "    netspeak4 serve -c /1.properties -c /2.properties\n"
            "\n"
            "Glob patterns are also supported. Example:\n"
            "    netspeak4 serve -c /*.properties\n"
            "\n"
            "Note that all indexes have to have unique coprus keys.");
  easy_init("port,p", bpo::value<uint16_t>()->required(),
            "The port on which the server will listen.");
  easy_init(
      "override,r", bpo::value<std::vector<std::string>>()->multitoken(),
      "A key-value pair that will override a properties in the given "
      "configuration file(s). You can specify any number of overrides\n"
      "\n"
      "All overrides have to be in the form:\n"
      "    --override \"key:value\"\n"
      "or:\n"
      "    --override \"key=value\"\n"
      "\n"
      "All overrides together will be treated similar to a configuration file. "
      "Every given configuration file (via `-c`) will be replaced with a new "
      "virtual file that contains all overrides and extends the given file.\n"
      "\n"
      "The only key that cannot be overriden is `extends`.");
}

boost::optional<Configuration> parse_overrides(
    const std::vector<std::string>& overrides) {
  util::Config config;

  for (auto& line : overrides) {
    auto separator_index = std::min(line.find(':'), line.find('='));
    if (separator_index == std::string::npos) {
      throw std::logic_error(
          "The given override is not valid. Expected a key-value pair but did "
          "not find a key-value separator (either `:` or `=`).\n"
          "Invalid override:\n" +
          line);
    }

    auto key = line.substr(0, separator_index);
    auto value = line.substr(separator_index + 1);
    boost::trim(key);
    boost::trim(value);

    if (key == Configuration::EXTENDS) {
      throw std::logic_error("The key `" + Configuration::EXTENDS +
                             "` connot be overridden.\n"
                             "Invalid override:\n" +
                             line);
    }

    config[key] = value;
  }

  if (config.empty()) {
    return boost::none;
  } else {
    return Configuration(config);
  }
}
Configuration load_config(const std::string& config_file,
                          const boost::optional<Configuration>& override) {
  std::cout << "Loading config " << config_file << "\n";
  Configuration config(config_file);
  if (override) {
    config = override->extend(config);
  }
  return config;
}

service::UniqueMap::entry load_map_entry(const Configuration& config) {
  service::Corpus corpus;
  corpus.set_key(config.get_required(Configuration::CORPUS_KEY));
  corpus.set_name(config.get_required(Configuration::CORPUS_NAME));
  corpus.set_language(config.get_required(Configuration::CORPUS_LANGUAGE));

  auto netspeak = std::make_unique<Netspeak>();
  netspeak->initialize(config);

  std::cout << "Loaded index " << corpus << "\n";

  return service::UniqueMap::entry{
    .instance = std::move(netspeak),
    .corpus = corpus,
  };
}


int ServeCommand::run(boost::program_options::variables_map variables) {
  const auto& patterns = variables["config"].as<std::vector<std::string>>();
  const auto overrides =
      variables.count("override") == 0
          ? boost::none
          : parse_overrides(
                variables["override"].as<std::vector<std::string>>());

  auto entries = std::make_unique<std::vector<service::UniqueMap::entry>>();
  for (const auto& pattern : patterns) {
    std::vector<std::string> paths;
    util::glob(pattern, paths);
    for (const auto& path : paths) {
      entries->push_back(load_map_entry(load_config(path, overrides)));
    }
  }
  service::UniqueMap service(std::move(entries));

  grpc::ServerBuilder builder;
  auto port = variables["port"].as<uint16_t>();
  builder.AddListeningPort("[::]:" + std::to_string(port),
                           grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port " << port << "\n";

  // For some reason, this flush is essential for the whole thing to work in
  // docker. I have no idea why but after about 30h of trial and error, it
  // finally worked and this is what made it work.
  std::cout.flush();

  server->Wait();

  return EXIT_SUCCESS;
}

} // namespace cli
