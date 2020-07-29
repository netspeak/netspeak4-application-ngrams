#include "cli/ShellCommand.hpp"

#include <cstdio>
#include <iomanip>

#include "boost/filesystem.hpp"

#include "netspeak/Configuration.hpp"
#include "netspeak/Netspeak.hpp"

namespace cli {

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
using namespace netspeak;

std::string ShellCommand::desc() {
  return "Load an interactive shell from a Netspeak index.";
};

void ShellCommand::add_options(
    boost::program_options::options_description_easy_init& easy_init) {
  easy_init("in,i", bpo::value<std::string>(),
            "Directory containing a Netspeak index.");
  easy_init(
      "config,c", bpo::value<std::string>(),
      "A .properties file describing the configuration of a Netspeak index.\n"
      "\n"
      "You have to provide either `--in` or `--config` but not both.");
}

/**
 * Runs an interactive Netspeak with the default retrieval strategy.
 */
void RunNetspeakShell(const Configuration& config) {
  Netspeak netspeak;
  try {
    netspeak.initialize(config);
  } catch (const std::exception& error) {
    return aitools::log(error.what());
  }

  // Set up a Netspeak request.
  service::SearchRequest request;
  request.set_max_phrases(100);

  while (true) {
    std::string query;
    std::cout << "\nEnter query (type 'q' to exit): ";
    std::getline(std::cin, query);
    if (query == "q") {
      std::cout << "Shutting down. This might take a while." << std::endl;
      break;
    }
    std::cout << std::endl;

    request.set_query(query);

    // Search Netspeak. This will never throw an exception.
    service::SearchResponse response;
    netspeak.search(request, response);

    if (response.has_result()) {
      const auto& result = response.result();

      // print all retrieved phrases
      size_t i = 1;
      std::cout << "Rank: Frequency    Text" << std::endl;
      for (const auto& phrase : result.phrases()) {
        std::cout << std::setw(4) << i++ << ": " << std::setw(12)
                  << phrase.frequency();
        for (const auto& word : phrase.words()) {
          std::cout << " " << word.text();
        }
        std::cout << std::endl;
      }
    } else {
      const auto& error = response.error();

      // print error
      std::cout << "Error: "
                << service::SearchResponse::Error::Kind_Name(error.kind())
                << ": " << error.message() << std::endl;
    }
    std::cout.flush();
  }
}

int ShellCommand::run(boost::program_options::variables_map variables) {
  Configuration config;

  if (variables.count("in") != 0 && variables.count("config") != 0) {
    throw std::runtime_error("You cannot provide both `--in` and `--config`.");
  } else if (variables.count("in") != 0) {
    config = {
      { Configuration::path_to_home, variables["in"].as<std::string>() },
      { Configuration::cache_capacity, "1000" },
    };
  } else if (variables.count("config") != 0) {
    config = Configuration(variables["config"].as<std::string>());
  } else {
    throw std::runtime_error(
        "You have to provide either `--in` or `--config`.");
  }

  RunNetspeakShell(config);
  return EXIT_SUCCESS;
}

} // namespace cli
