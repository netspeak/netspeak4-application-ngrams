TEMPLATE = lib
TARGET = netspeak4
VERSION = 1.3.8

QMAKE_CXXFLAGS += -fPIC

COMMON_INCLUDE = netspeak4-application-cpp.pri
!include(../../$$COMMON_INCLUDE) {
  error("Could not find $$COMMON_INCLUDE file")
}
