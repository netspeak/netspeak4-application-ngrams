TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
TARGET = netspeak4-test

COMMON_INCLUDE = netspeak4-application-cpp.pri
!include(../../$$COMMON_INCLUDE) {
  error("Could not find $$COMMON_INCLUDE file")
}

SOURCES += ../../test/netspeak/*.cpp
