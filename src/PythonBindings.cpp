#include <pybind11/pybind11.h>

#include <memory>

#include "py/QueryParser.hpp"

using namespace py;


PYBIND11_MODULE(example, m) {
  m.doc() = "Netspeak4 Python bindings"; // optional module docstring

  pybind11::class_<NormQueryUnit> normQueryUnit(m, "NormQueryUnit");

  normQueryUnit
      .def(pybind11::init<NormQueryUnit::Kind, std::string,
                          NormQueryUnit::QueryUnitKind>())
      .def("getKind", &NormQueryUnit::get_kind)
      .def("getText", &NormQueryUnit::get_kind)
      .def("getSourceKind", &NormQueryUnit::get_kind);

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
}
