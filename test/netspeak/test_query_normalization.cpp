#include <iostream>
#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "netspeak/query_methods.hpp"

// Boost.Test needs operator<< for each involved type.
namespace std {

inline ostream& operator<<(ostream& os, const vector<string>& vector) {
  copy(vector.begin(), vector.end(), ostream_iterator<string>(os, ", "));
  return os;
}

inline ostream& operator<<(ostream& os, const vector<vector<string>>& matrix) {
  copy(matrix.begin(), matrix.end(),
       ostream_iterator<vector<string>>(os, "\n"));
  return os;
}

inline ostream& operator<<(ostream& os, const set<string>& set) {
  copy(set.begin(), set.end(), ostream_iterator<string>(os, ", "));
  return os;
}

/*inline ostream& operator<<(ostream& os,
                           const vector<netspeak::generated::Query>& queries) {
  copy(queries.begin(), queries.end(),
       ostream_iterator<netspeak::generated::Query>(os, "\n"));
  return os;
}*/

} // namespace std
