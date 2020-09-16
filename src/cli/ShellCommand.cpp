#include "cli/ShellCommand.hpp"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <cstdio>
#include <iomanip>

#include "boost/filesystem.hpp"

#include "netspeak/Configuration.hpp"
#include "netspeak/Netspeak.hpp"
#include "netspeak/service/NetspeakService.grpc.pb.h"
#include "netspeak/util/service.hpp"

namespace cli {

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
using namespace netspeak;

std::string ShellCommand::desc() {
  return "Load an interactive shell from a Netspeak index.";
};

void ShellCommand::add_options(
    boost::program_options::options_description_easy_init& easy_init) {
  easy_init(
      "in,i", bpo::value<std::string>(),
      "Directory containing a Netspeak index.\n"
      "\n"
      "You have to provide exactly one of `--config`, `--in`, or `--port`.");
  easy_init(
      "config,c", bpo::value<std::string>(),
      "A .properties file describing the configuration of a Netspeak index.\n"
      "\n"
      "You have to provide exactly one of `--config`, `--in`, or `--port`.");
  easy_init(
      "port,p", bpo::value<uint16_t>(),
      "The port of a local Netspeak server.\n"
      "\n"
      "You have to provide exactly one of `--config`, `--in`, or `--port`.");
  easy_init("corpus", bpo::value<std::string>(),
            "The corpus key used for all requests to a Netspeak server.\n"
            "\n"
            "This can only be used in combination with the `--port` option.");
}

typedef std::function<void(const service::SearchRequest&,
                           service::SearchResponse&)>
    Searcher;

void RunShell(Searcher& searcher) {
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
    searcher(request, response);

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

/**
 * Runs an interactive Netspeak with the default retrieval strategy.
 */
void RunNetspeakShell(const Configuration& config) {
  Netspeak netspeak;
  try {
    netspeak.initialize(config);
  } catch (const std::exception& error) {
    aitools::log(error.what());
    return;
  }

  Searcher searcher = [&](const service::SearchRequest& request,
                          service::SearchResponse& response) {
    netspeak.search(request, response);
  };
  RunShell(searcher);
}

void handle_corpus_key(std::string& corpus_key,
                       netspeak::service::NetspeakService::Stub& stub) {
  // query all corpora
  service::CorporaRequest corporaReq;
  service::CorporaResponse corporaRes;

  grpc::ClientContext context;
  auto corpora_status = stub.GetCorpora(&context, corporaReq, &corporaRes);
  if (!corpora_status.ok()) {
    std::stringstream what;
    what << "Couldn't get corpora.\n" << corpora_status;
    throw std::logic_error(what.str());
  }

  const auto& corpora = corporaRes.corpora();
  if (corpus_key.empty()) {
    // select key

    if (corpora.empty()) {
      throw std::logic_error("The server doesn't support any corpora.");
    } else if (corpora.size() == 1) {
      const auto& corpus = *corpora.begin();
      std::cout << "The corpus " << corpus
                << " has been selected because it's the only corpus.\n";
      corpus_key = corpus.key();
    } else {
      std::cout << "The server supports multiple corpora. Please chose one by "
                   "entering its number:\n";
      int i = 0;
      for (const auto& corpus : corpora) {
        std::cout << "\t" << (i++) << ": " << corpus << "\n";
      }
      int index;
      std::cin >> index;
      if (index < 0 || index >= corpora.size()) {
        throw std::logic_error("Number out of range.");
      }

      const auto& corpus = corporaRes.corpora(index);
      std::cout << "The corpus " << corpus << " has been selected.\n";
      corpus_key = corpus.key();
    }
  } else {
    // verify key

    if (std::find_if(corpora.begin(), corpora.end(),
                     [&](const service::Corpus& corpus) {
                       return corpus.key() == corpus_key;
                     }) == corpora.end()) {
      std::stringstream what;
      what << "Cannot find the given corpus key \"" << corpus_key
           << "\" in the list of corpora support by the server.\n"
           << "Supported are:";
      for (const auto& corpus : corpora) {
        what << "\n\t" << corpus;
      }
      throw std::logic_error(what.str());
    }
  }
}
void RunLocalServerShell(uint16_t port, std::string corpus_key) {
  const std::string address = "localhost:" + std::to_string(port);
  auto channel =
      grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
  auto stub = service::NetspeakService::NewStub(channel);

  handle_corpus_key(corpus_key, *stub);

  Searcher searcher = [&](const service::SearchRequest& request,
                          service::SearchResponse& response) {
    service::SearchRequest req(request);
    req.set_corpus(corpus_key);

    grpc::ClientContext context;
    auto status = stub->Search(&context, req, &response);
    if (!status.ok()) {
      std::stringstream what;
      what << status;
      throw std::logic_error(what.str());
    }
  };
  RunShell(searcher);
}

int ShellCommand::run(boost::program_options::variables_map variables) {
  Configuration config;

  size_t input_options = 0;
  if (variables.count("in") != 0)
    input_options++;
  if (variables.count("config") != 0)
    input_options++;
  if (variables.count("port") != 0)
    input_options++;

  if (input_options != 1) {
    std::stringstream msg;
    msg << "You have to provide exactly one of `--config`, `--in`, or "
           "`--port`.\n"
        << "You provided " << input_options << " options.";
    throw std::runtime_error(msg.str());
  }

  if (variables.count("in") != 0) {
    config = {
      { Configuration::path_to_home, variables["in"].as<std::string>() },
      { Configuration::cache_capacity, "1000" },
    };
  } else if (variables.count("config") != 0) {
    config = Configuration(variables["config"].as<std::string>());
  } else /* if (variables.count("port") != 0) */ {
    auto port = variables["port"].as<uint16_t>();
    std::string corpus_key;
    if (variables.count("corpus") != 0) {
      corpus_key = variables["corpus"].as<std::string>();
    }

    RunLocalServerShell(port, corpus_key);
    return EXIT_SUCCESS;
  }

  RunNetspeakShell(config);
  return EXIT_SUCCESS;
}

} // namespace cli
