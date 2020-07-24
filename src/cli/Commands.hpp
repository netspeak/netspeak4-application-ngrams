#ifndef CLI_COMMANDS_HPP
#define CLI_COMMANDS_HPP

#include <memory>
#include <vector>

#include "cli/Command.hpp"


namespace cli {

class Commands {
private:
  std::vector<std::unique_ptr<Command>> _commands;

public:
  void add_command(std::unique_ptr<Command> command) {
    _commands.push_back(std::move(command));
  }

  int run(int argc, char* argv[]);
};

} // namespace cli

#endif
