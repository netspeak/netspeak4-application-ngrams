#ifndef PATHS_HPP
#define PATHS_HPP

#include <fstream>
#include <netspeak/error.hpp>
#include <netspeak/util/check.hpp>
#include <string>

namespace test {

const std::string INDEX_DIR = "data/index_eng500k_ci";

const std::string QUERIES_DIR = "data/queries";

const std::string DICTIONARY_SET = "data/dictset.csv";

const std::string REGEX_1K_LOWER = "data/regex_1k_lower.txt";

const std::string TEST_REGEX_DATA = "data/test_Regex.txt";


inline std::string load_file(std::string path) {
  std::ifstream ifs(path);
  netspeak::util::check(ifs.is_open(), netspeak::error_message::cannot_open, path);
  std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  ifs.close();
  return content;
}

} // namespace test


#endif // PATHS_HPP
