#include <memory>

#include "cli/BuildCommand.hpp"
#include "cli/Commands.hpp"
#include "cli/ProxyCommand.hpp"
#include "cli/RegexCommand.hpp"
#include "cli/ServeCommand.hpp"
#include "cli/ShellCommand.hpp"

int main(int argc, char** argv) {
  cli::Commands commands;

  commands.add_command(std::make_unique<cli::BuildCommand>());
  commands.add_command(std::make_unique<cli::ProxyCommand>());
  commands.add_command(std::make_unique<cli::RegexCommand>());
  commands.add_command(std::make_unique<cli::ServeCommand>());
  commands.add_command(std::make_unique<cli::ShellCommand>());

  commands.run(argc, argv);
}
