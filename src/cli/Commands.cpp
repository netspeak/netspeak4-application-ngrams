#include "cli/Commands.hpp"

#include <iostream>
#include <string>

#include "boost/program_options.hpp"

namespace cli {

namespace bpo = boost::program_options;

/**
 * @brief Print a short help message that lists all available commands.
 *
 * @param commands
 */
void print_commands_help(
    const std::vector<std::unique_ptr<Command>>& commands) {
  size_t max_name_len = 0;
  for (const auto& command : commands) {
    max_name_len = std::max(max_name_len, command->name().size());
  }

  std::cout << "General usage: \"netspeak4 COMMAND ...\"\n"
            << "where COMMANDS is one of:\n"
            << "\n";

  for (const auto& command : commands) {
    const auto& name = command->name();
    const auto& desc = command->desc();
    auto padding = max_name_len - name.size() + 4;
    std::cout << "  " << name << std::string(padding, ' ') << desc << "\n";
  }

  std::cout << "\n"
            << "Run \"netspeak4 COMMAND --help\" to get command-specific usage "
               "information.\n";
}

bpo::options_description get_option_desc(Command& command) {
  bpo::options_description desc(command.desc());

  auto easy_init = desc.add_options();
  easy_init("help,h", "Print help message");
  command.add_options(easy_init);

  return desc;
}

void print_command_help(Command& command) {
  std::cout << "General usage of \"netspeak4 " << command.name() << " ...\"\n"
            << "\n";
  auto options = get_option_desc(command);
  options.print(std::cout, 80);
  std::cout << "\n";
}

void print_error(const std::vector<std::unique_ptr<Command>>& commands,
                 const std::string& message) {
  std::cerr << "Error: " << message << "\n"
            << "\n";
  print_commands_help(commands);
}
void print_error(Command& command, const std::string& message) {
  std::cerr << "Error: " << message << "\n"
            << "\n";
  print_command_help(command);
}

int run_command(Command& command, std::vector<std::string> opts) {
  try {
    auto options_desc = get_option_desc(command);

    bpo::variables_map variables;
    bpo::store(bpo::command_line_parser(opts).options(options_desc).run(),
               variables);

    if (variables.count("help")) {
      print_command_help(command);
      return EXIT_SUCCESS;
    }

    return command.run(std::move(variables));
  } catch (std::exception& e) {
    print_error(command, e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Commands::run(int argc, char* argv[]) {
  if (argc < 2) {
    print_error(commands_, "Not enough arguments.");
    return EXIT_FAILURE;
  }

  try {
    bpo::options_description global("");
    global.add_options()("command", bpo::value<std::string>(),
                         "command to execute")(
        "subargs", bpo::value<std::vector<std::string>>(),
        "Arguments for command");

    bpo::positional_options_description pos_opt_desc;
    pos_opt_desc.add("command", 1).add("subargs", -1);

    auto parsed_options = bpo::command_line_parser(argc, argv)
                              .options(global)
                              .positional(pos_opt_desc)
                              .allow_unregistered()
                              .run();

    bpo::variables_map variables;
    bpo::store(parsed_options, variables);

    std::string command_name = variables["command"].as<std::string>();

    for (const auto& command_ptr : commands_) {
      auto& command = *command_ptr;
      if (command.name() == command_name) {
        // collect all arguments
        std::vector<std::string> opts = bpo::collect_unrecognized(
            parsed_options.options, bpo::include_positional);
        if (!opts.empty()) {
          opts.erase(opts.begin());
        }

        try {
          return run_command(command, opts);
        } catch (std::exception& e) {
          std::cerr << "Error: " << e.what() << "\n";
          return EXIT_FAILURE;
        }
      }
    }

    print_error(commands_, "Unknown command \"" + command_name + "\".");
    return EXIT_FAILURE;
  } catch (std::exception& e) {
    print_error(commands_, e.what());
    return EXIT_FAILURE;
  }
}

} // namespace cli
