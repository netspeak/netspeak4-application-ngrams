#include <dirent.h>
#include <stdio.h>

#include <fstream>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

#include "antlr4-runtime.h"
#include "antlr4/QueryErrorHandler.hpp"
#include "antlr4/RawQueryListener.hpp"
#include "antlr4/generated/QueryLexer.h"
#include "antlr4/generated/QueryParser.h"

#include "netspeak/Dictionaries.hpp"
#include "netspeak/error.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"

using namespace antlr4;

BOOST_AUTO_TEST_CASE(test_detection) {
  std::string path(test::QUERIES_DIR);

  DIR *dir = opendir(path.c_str());

  if (dir == NULL) {
    std::cerr << "no such directory" << std::endl;
    return;
  }
  struct dirent *pdir;

  std::vector<std::string> files;

  while ((pdir = readdir(dir)) != NULL) {
    files.push_back(path + "/" + pdir->d_name);
  }
  closedir(dir);
  for (auto file = files.begin(); file != files.end(); file++) {
    std::ifstream inputStream(*file, std::ios::in);

    std::string read;

    while (inputStream.peek() != -1) {
      std::getline(inputStream, read);

      size_t pos = read.find("\t");
      std::string query = read.substr(0, pos);
      std::string expected = read.substr(pos + 4, read.size());

      antlr4::ANTLRInputStream input(query);
      QueryLexer lexer(&input);
      antlr4::CommonTokenStream tokens(&lexer);
      QueryParser parser(&tokens);
      parser.removeErrorListeners();
      parser.addErrorListener(&antlr4::QueryErrorHandler::INSTANCE);

      std::string actual;
      try {
        antlr4::tree::ParseTree *tree = parser.query();
        actual = tree->toStringTree(&parser);
      } catch (...) {
        actual = "FAIL";
      }

      BOOST_REQUIRE_EQUAL(actual, expected);
    }
    inputStream.close();
  }
}
