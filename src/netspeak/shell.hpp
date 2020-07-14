#ifndef NETSPEAK_SHELL_HPP
#define NETSPEAK_SHELL_HPP

#include <cstdio>
#include <memory>
#include <string>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>

#include "aitools/util/logging.hpp"

#include "netspeak/Configurations.hpp"
#include "netspeak/Netspeak.hpp"

namespace netspeak {

/**
 * Runs an interactive Netspeak with the default retrieval strategy.
 */
void RunNetspeakShell(const std::string& home_dir) {
  Netspeak netspeak;
  try {
    const Configurations::Map config = {
      { Configurations::path_to_home, home_dir },
      { Configurations::cache_capacity, "1000" }
    };
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

} // namespace netspeak

#endif
