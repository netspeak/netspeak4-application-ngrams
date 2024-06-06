#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>

#include "py/QueryParser.hpp"

using namespace py;


PYBIND11_MODULE(netspeak4py, m) {
  m.doc() = "Netspeak4 Python bindings"; // optional module docstring

  pybind11::class_<NormQueryUnit> normQueryUnit(m, "NormQueryUnit");
  normQueryUnit
      .def(pybind11::init<NormQueryUnit::Kind, std::string,
                          NormQueryUnit::QueryUnitKind>(),
           "Create a new norm query unit", pybind11::arg("kind"),
           pybind11::arg("text"), pybind11::arg("sourceKind"))
      .def("getKind", &NormQueryUnit::get_kind)
      .def("getText", &NormQueryUnit::get_text)
      .def("getSourceKind", &NormQueryUnit::get_source_kind)
      .def("__repr__", [](const NormQueryUnit& u) {
        const auto kind = u.get_kind();
        const std::string& kind_str = kind == NormQueryUnit::Kind::WORD ? "NormWord"
                               : kind == NormQueryUnit::Kind::QMARK
                                   ? "NormQMark"
                                   : "Invalid";

        return "<netspeak4py.NormQueryUnit " + kind_str + " \"" +
               (u.get_text()) + "\">";
      });

  pybind11::enum_<NormQueryUnit::Kind>(normQueryUnit, "Kind")
      .value("NormWord", NormQueryUnit::Kind::WORD)
      .value("NormQMark", NormQueryUnit::Kind::QMARK)
      .export_values();

  pybind11::enum_<NormQueryUnit::QueryUnitKind>(normQueryUnit, "QueryUnitKind")
      .value("WORD", NormQueryUnit::QueryUnitKind::WORD)
      .value("QMARK", NormQueryUnit::QueryUnitKind::QMARK)
      .value("STAR", NormQueryUnit::QueryUnitKind::STAR)
      .value("PLUS", NormQueryUnit::QueryUnitKind::PLUS)
      .value("REGEX", NormQueryUnit::QueryUnitKind::REGEX)
      .value("DICTSET", NormQueryUnit::QueryUnitKind::DICTSET)
      .value("ORDERSET", NormQueryUnit::QueryUnitKind::ORDERSET)
      .value("OPTIONSET", NormQueryUnit::QueryUnitKind::OPTIONSET)
      .value("ALTERNATION", NormQueryUnit::QueryUnitKind::ALTERNATION)
      .value("CONCAT", NormQueryUnit::QueryUnitKind::CONCAT)
      .export_values();

  pybind11::class_<NormQuery> normQuery(m, "NormQuery");
  normQuery.def(pybind11::init<>())
      .def("getUnits", &NormQuery::get_units)
      .def("__repr__", [](NormQuery& q) {
        std::string s = "<netspeak4py.NormQuery \"";
        auto first = true;
        for (const auto& u : q.get_units()) {
          if (first) {
            first = false;
          } else {
            s.push_back(' ');
          }

          s.append(u.get_text());
        }

        s.append("\">");
        return s;
      });
  ;

  pybind11::class_<QueryParserOptions> queryParserOptions(m,
                                                          "QueryParserOptions");
  queryParserOptions.def(pybind11::init<>())
      .def_readwrite("maxNormQueries", &QueryParserOptions::max_norm_queries)
      .def_readwrite("minLength", &QueryParserOptions::min_length)
      .def_readwrite("maxLength", &QueryParserOptions::max_length)
      .def_readwrite("maxRegexMatches", &QueryParserOptions::max_regex_matches)
      .def_readwrite("maxRegexTimeMs", &QueryParserOptions::max_regex_time_ms);

  pybind11::class_<QueryParser> queryParser(m, "QueryParser");
  queryParser
      .def(pybind11::init<const std::string&, const std::string&, bool>(),
           "Creates a new query parser.", pybind11::arg("vocab") = "",
           pybind11::arg("dictionaryCsv") = "",
           pybind11::arg("lowerCase") = false)
      .def("parse", &QueryParser::parse,
           "Parses the given Netspeak 4 query and returns a list of norm "
           "queries.",
           pybind11::arg("query"), pybind11::arg("options"));
}
