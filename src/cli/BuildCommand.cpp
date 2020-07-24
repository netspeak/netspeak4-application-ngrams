#include "cli/BuildCommand.hpp"

#include "boost/filesystem.hpp"

#include "netspeak/indexing.hpp"

namespace cli {

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

void BuildCommand::add_options(
    boost::program_options::options_description_easy_init& easy_init) {
  easy_init("in,i", bpo::value<std::string>()->required(), "Input directory");
  easy_init("out,o", bpo::value<std::string>()->required(),
            "Output directory (must be empty)");
  easy_init("merge-duplicates,md",
            "If your n-gram collection is not unique, you have to merge");
  easy_init("merge-dir", bpo::value<std::string>(),
            "Overrides default tmp dir");
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
int BuildCommand::run(boost::program_options::variables_map variables) {
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

    netspeak::MergeDuplicates(input_dir, tmp_dir);
    input_dir = tmp_dir;
    output_dir /= "tmp-index";
  }

  netspeak::BuildNetspeak(input_dir, output_dir);
  if (org_input_dir != input_dir.c_str()) {
    if (input_dir.parent_path() == org_input_dir) {
      bfs::remove_all(input_dir);
    }
    bfs::rename(output_dir, bfs::path(org_output_dir));
  }

  return EXIT_SUCCESS;
}

} // namespace cli
