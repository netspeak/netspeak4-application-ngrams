#include <fstream>
#include <netspeak/Dictionaries.hpp>
#include <netspeak/error.hpp>
#include <netspeak/util/check.hpp>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace netspeak::Dictionaries {

const Map read_from_file(const std::filesystem::path& csv) {
  std::ifstream ifs(csv);
  util::check(ifs.is_open(), error_message::cannot_open, csv);
  return parse_csv(ifs);
}

const Map parse_csv(std::istream& stream) {
  Map dict;
  std::string line;
  std::vector<std::string> tokens;
  /** Equivalence relations are symmetric; why do we specifically bind the second argument to '\t' and not the first? */
  const auto predicate = std::bind(std::equal_to<char>(), std::placeholders::_1, '\t');
  while (std::getline(stream, line)) {
    boost::split(tokens, line, predicate);
    if (tokens.size() < 2)
      continue;
    for (unsigned i = 1; i != tokens.size(); ++i) {
      dict.insert(std::make_pair(tokens.front(), tokens[i]));
    }
  }
  return dict;
}

} // namespace netspeak::Dictionaries
