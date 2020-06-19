#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <cstdio>
#include <memory>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "antlr4-runtime.h"
#include "antlr4/QueryErrorHandler.hpp"
#include "antlr4/QueryLengthVerifier.hpp"
#include "antlr4/QueryNormListener.hpp"
#include "antlr4/RawQueryListener.hpp"
#include "antlr4/generated/QueryLexer.h"
#include "antlr4/generated/QueryParser.h"

#include "netspeak/error.hpp"
#include "netspeak/indexing.hpp"
#include "netspeak/regex/DefaultRegexIndex.hpp"
#include "netspeak/regex/parsers.hpp"
#include "netspeak/shell.hpp"
#include "netspeak/NetspeakServiceImpl.hpp"


namespace netspeak {

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

// -----------------------------------------------------------------------------
// This example shows how to search Netspeak.
// -----------------------------------------------------------------------------
int HowToSearchNetspeak() {
  // Set up the Netspeak configuration.
  // home_dir is the directory defined by dst_dir_bn in howto_build_netspeak.
  const Configurations::Map config = {
    { Configurations::path_to_home,
      "/home/temir/Schreibtisch/data-in-production/netspeak/netspeak3-web-en" },
    { Configurations::cache_capacity, "1000" }
  };

  // Start Netspeak. This should happen only once in the initialization code
  // of your application. If not stated otherwise, use RetrievalStrategy3 as
  // the latest and best performing retrieval strategy. Depending on the size
  // of the n-gram collection the initialization can take some seconds.
  Netspeak<RetrievalStrategy3Tag> netspeak;
  try {
    netspeak.initialize(config);
  } catch (std::exception& error) {
    std::cerr << "Some error occured: " << error.what() << std::endl;
  }

  // Set up a Netspeak request.
  generated::Request request;
  while (true) {
    std::cout << "Insert a netspeak query ('q' to exit): ";
    char input[10000];
    std::cin.getline(input, sizeof(input));

    std::string in = input;
    if (in == "q")
      break;

    request.set_query(in);
    request.set_max_phrase_count(100); // Optional: Get the top ten results.

    // Search Netspeak. This call do never throw an exception.
    std::shared_ptr<generated::Response> response = netspeak.search(request);

    // Check if any error occurred.
    if (to_error_code(response->error_code()) != error_code::no_error) {
      std::printf("Error: %s : %s\n",
                  to_string(to_error_code(response->error_code())).c_str(),
                  response->error_message().c_str());
    } else {
      // Print all retrieved phrases.

      for (const auto& phrase : response->phrase()) {
        std::printf("%-13lu %s\n", phrase.frequency(),
                    to_string(phrase).c_str());
      }
    }
  }

  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// Prints generic program information if no specific command is selected
// -----------------------------------------------------------------------------
void PrintGenericHelp() {
  std::cout
      << "Usage: netspeak COMMAND\n"
         "where COMMAND is one of:\n"
         "  build\t\tCreate a new Netspeak index with given ngrams\n"
         "  shell\t\tLoad an interactive shell from Netspeak index\n"
         "  regex\t\tLoad an interactive shell from regex list\n"
         "Run \"netspeak COMMAND --help\" to get specific usage informations"
      << std::endl;
}

// -----------------------------------------------------------------------------
// Builds netspeak index with given command line options.
// An n-gram collection consists of a set of n-gram files, whose lines must be
// formatted as follows (other formats are not supported):
//
// <n-gram> TAB <n-gram-frequency>
//
// The separator between the n-gram and its frequency is a single tab ('\t').
// The separator between the n-grams words is a single whitespace (' ').
// Example:
//
// hello world 123
// -----------------------------------------------------------------------------
int RunBuild(const std::vector<std::string>& opts) {
  bpo::options_description desc(
      "Builds a new `netspeak` from n-grams. "
      "If your n-gram collection is not unique, you have to merge. "
      "When merging the directory `netspeak-merged` will be added to `from` "
      "directories parent folder. Use `-merge-dir` to "
      "override this behaviour");

  desc.add_options()("help,h", "Print help message")(
      "in,i", bpo::value<std::string>()->required(), "Input directory")(
      "out,o", bpo::value<std::string>()->required(),
      "Output directory (must be empty)")(
      "merge-duplicates,md",
      "If your n-gram collection is not unique, you have to merge")(
      "merge-dir", bpo::value<std::string>(), "Overrides default tmp dir");

  try {
    bpo::variables_map variables;
    bpo::store(bpo::command_line_parser(opts).options(desc).run(), variables);

    if (variables.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    bpo::notify(variables);

    const std::string org_input_dir = variables["in"].as<std::string>();
    const std::string org_output_dir = variables["out"].as<std::string>();

    bfs::path input_dir(org_input_dir);
    bfs::path output_dir(org_output_dir);

    if (variables.count("merge-duplicates")) {
      // if keys have to be merged and no merge dir was given. Create 'tmp'
      // dir in dst_folder
      bfs::path tmp_dir =
          variables.count("merge-dir")
              ? bfs::path(variables["merge-dir"].as<std::string>())
              : output_dir / bfs::path("tmp");

      MergeDuplicates(input_dir, tmp_dir);
      input_dir = tmp_dir;
      output_dir /= "tmp-index";
    }

    BuildNetspeak(input_dir, output_dir);
    if (org_input_dir != input_dir.c_str()) {
      if (input_dir.parent_path() == org_input_dir) {
        bfs::remove_all(input_dir);
      }
      bfs::rename(output_dir, bfs::path(org_output_dir));
    }
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl << desc << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// Start a netspeak CLI with command line options
// -----------------------------------------------------------------------------
int RunShell(const std::vector<std::string>& opts) {
  bpo::options_description desc("Open shell for an existing netspeak index");
  desc.add_options()("help,h", "Print help message")(
      "in,i", bpo::value<std::string>()->required(),
      "Directory containing `netspeak` index");

  try {
    bpo::variables_map variables;
    bpo::store(bpo::command_line_parser(opts).options(desc).run(), variables);

    if (variables.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    bpo::notify(variables);

    RunNetspeakShell(variables["in"].as<std::string>());
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl << desc << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

std::string load_file(std::string path) {
  std::ifstream ifs(path);
  aitools::check(ifs.is_open(), netspeak::error_message::cannot_open, path);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  ifs.close();
  return content;
}

// -----------------------------------------------------------------------------
// Start a netspeak regex CLI with command line options
// -----------------------------------------------------------------------------
int RunRegex(const std::vector<std::string>& opts) {
  bpo::options_description desc("Open shell for an existing netspeak index");
  desc.add_options()("help,h", "Print help message")(
      "in,i", bpo::value<std::string>()->required(), "File of the regex list");

  try {
    bpo::variables_map variables;
    bpo::store(bpo::command_line_parser(opts).options(desc).run(), variables);

    if (variables.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    bpo::notify(variables);

    const auto file = variables["in"].as<std::string>();
    const auto string = load_file(file);

    std::cout << "Constructing regex index...\n";
    const auto init_start = std::chrono::steady_clock::now();
    const netspeak::regex::DefaultRegexIndex index(string);
    const auto init_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - init_start);
    std::cout << "Took " << init_time.count()
              << "ms to construtct the regex index.\n";

    while (true) {
      std::string query;
      std::cout << "\nEnter query (empty query to exit): ";
      std::getline(std::cin, query);
      if (query.empty()) {
        std::cout << "Exit." << std::endl;
        break;
      }

      const auto regex_query =
          netspeak::regex::parse_netspeak_regex_query(query);
      const auto timeout = std::chrono::milliseconds(1000);
      std::vector<std::string> matches;


      const auto match_start = std::chrono::steady_clock::now();
      index.match_query(regex_query, matches, 20, timeout);
      const auto match_time =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now() - match_start);

      std::cout << "Found " << matches.size() << " matche(s) in "
                << match_time.count() << "ms:\n";
      for (size_t i = 0; i < matches.size(); i++) {
        std::cout << "  " << std::setw(4) << std::right << (i + 1) << "  "
                  << std::setw(60) << std::left << matches[i] << "\n";
      }
    }

  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl << desc << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int RunServe(const std::vector<std::string>& opts) {
  bpo::options_description desc(
      "Creates a new server of the given Netspeak index.");
  desc.add_options()("help,h", "Print help message")(
      "config,c", bpo::value<std::string>()->required(),
      "The configuration file of the index")(
      "port,p", bpo::value<uint16_t>()->required(),
      "The port on which the server will listen");

  try {
    bpo::variables_map variables;
    bpo::store(bpo::command_line_parser(opts).options(desc).run(), variables);

    if (variables.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    bpo::notify(variables);

    auto config_file = variables["config"].as<std::string>();
    auto port = variables["port"].as<uint16_t>();
    std::string server_address("localhost:");
    server_address.extend(std::to_string(port));

    NetspeakServiceImpl service(config_file);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl << desc << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// Entry point of the `netspeak` command line program.
// -----------------------------------------------------------------------------
int Run(int argc, char* argv[]) {
  // Examples.
  // $ netspeak build --from=/path/to/ngram/dir --to=/path/to/netspeak/dir
  // $ netspeak stats --dir=/path/to/netspeak/dir
  // $ netspeak shell --dir=/path/to/netspeak/dir
  if (argc < 2) {
    PrintGenericHelp();
    return EXIT_FAILURE;
  }

  try {
    bpo::options_description global("");
    global.add_options()("command", bpo::value<std::string>(),
                         "command to execute")(
        "subargs", bpo::value<std::vector<std::string> >(),
        "Arguments for command");

    bpo::positional_options_description pos_opt_desc;
    pos_opt_desc.add("command", 1).add("subargs", -1);

    bpo::parsed_options parsed_options = bpo::command_line_parser(argc, argv)
                                             .options(global)
                                             .positional(pos_opt_desc)
                                             .allow_unregistered()
                                             .run();

    bpo::variables_map variables;
    bpo::store(parsed_options, variables);

    std::string command = variables["command"].as<std::string>();

    std::vector<std::string> opts = bpo::collect_unrecognized(
        parsed_options.options, bpo::include_positional);
    opts.erase(opts.begin());

    if (command == "build")
      return RunBuild(opts);
    if (command == "shell")
      return RunShell(opts);
    if (command == "regex")
      return RunRegex(opts);
    if (command == "serve")
      return RunServe(opts);

    throw bpo::invalid_option_value(command);
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    PrintGenericHelp();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

} // namespace netspeak

// -----------------------------------------------------------------------------
// Runs an interactive terminal to search Netspeak on command line.
// The function run_netspeak_terminal can also be used by users of this library
// to quickly test their local Netspeak. It basically runs similar code as in
// howto_search_netspeak in a loop.
// -----------------------------------------------------------------------------


int main(int argc, char** argv) {
  return netspeak::Run(argc, argv);
}
