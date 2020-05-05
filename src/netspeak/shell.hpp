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
#include "netspeak/RetrievalStrategy1.hpp"
#include "netspeak/RetrievalStrategy2.hpp"
#include "netspeak/RetrievalStrategy3.hpp"

namespace netspeak {

template <typename RetrievalStrategyTag>
void RunNetspeakShellGeneric(const boost::filesystem::path& home_dir) {
  Netspeak<RetrievalStrategyTag> netspeak;
  try {
    const Configurations::Map config = {
      { Configurations::path_to_home, home_dir.string() },
      { Configurations::cache_capacity, "1000" }
    };
    netspeak.initialize(config);
  } catch (const std::exception& error) {
    return aitools::log(error.what());
  }

  while (true) {
    std::string query;
    std::cout << "\nEnter query (type 'q' to exit): ";
    std::getline(std::cin, query);
    if (query == "q") {
      std::cout << "Shutting down. This might take a while." << std::endl;
      break;
    }

    generated::Request request;
    request.set_query(query);

    //    boost::timer::auto_cpu_timer timer;
    std::shared_ptr<generated::Response> response = netspeak.search(request);
    if (response->error_code() == to_ordinal(error_code::no_error)) {
      if (response->phrase_size() == 0) {
        std::cout << "\nNothing found\n";
      } else {
        // Find longest phrase.
        std::size_t max_length = 0;
        for (const auto& phrase : response->phrase()) {
          max_length = std::max(max_length, to_text(phrase).size());
        }
        // Print table.
        const std::size_t colwidth_1 = 5;
        const std::size_t colwidth_2 = max_length + 2;
        const std::size_t colwidth_3 = 15;
        const std::size_t colwidth_4 = 15;
        const std::size_t tablewidth =
            colwidth_1 + colwidth_2 + colwidth_3 + colwidth_4;
        std::cout << '\n'
                  << std::setw(colwidth_1) << std::left << '#'
                  << std::setw(colwidth_2) << "ngram" << std::setw(colwidth_3)
                  << std::right << "frequency" << std::setw(colwidth_4) << "id"
                  << '\n'
                  << std::string(tablewidth, '-') << '\n';
        for (int i = 0; i != response->phrase_size(); ++i) {
          const generated::Phrase& phrase = response->phrase(i);
          std::cout << std::setw(colwidth_1) << std::left << (i + 1)
                    << std::setw(colwidth_2) << to_text(phrase)
                    << std::setw(colwidth_3) << std::right << phrase.frequency()
                    << std::setw(colwidth_4) << phrase.id() << '\n';
        }
      }
    } else {
      std::printf("Error: %s : %s\n",
                  to_string(to_error_code(response->error_code())).c_str(),
                  response->error_message().c_str());
    }
    std::cout.flush();
    //    std::printf("Mass:  %lu\n", response->total_frequency());
    //    std::printf("Time:  %f\n", timer.elapsed());

    //    std::cout << "Tokenized query: "
    //              << boost::algorithm::join(response->query_token(), " ")
    //              << std::endl;
    //    std::cout << "Parsed query: " << response->query().DebugString()
    //              << std::endl;
  }
}

/**
 * Runs an interactive Netspeak RS1 promt on command line.
 */
void RunNetspeakShellRS1(const boost::filesystem::path& home_dir) {
  RunNetspeakShellGeneric<RetrievalStrategy1Tag>(home_dir);
}

/**
 * Runs an interactive Netspeak RS2 promt on command line.
 */
void RunNetspeakShellRS2(const boost::filesystem::path& home_dir) {
  RunNetspeakShellGeneric<RetrievalStrategy2Tag>(home_dir);
}

/**
 * Runs an interactive Netspeak RS3 promt on command line.
 */
void RunNetspeakShellRS3(const boost::filesystem::path& home_dir) {
  RunNetspeakShellGeneric<RetrievalStrategy3Tag>(home_dir);
}

/**
 * Runs an interactive Netspeak with the default retrieval strategy.
 */
void RunNetspeakShell(const std::string& home_dir) {
  RunNetspeakShellRS3(boost::filesystem::path(home_dir));
}

} // namespace netspeak

#endif // NETSPEAK_SHELL_HPP
