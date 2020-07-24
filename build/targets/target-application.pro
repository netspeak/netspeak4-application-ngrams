TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
TARGET = netspeak4

COMMON_INCLUDE = netspeak4-application-cpp.pri
!include(../../$$COMMON_INCLUDE) {
  error("Could not find $$COMMON_INCLUDE file")
}

SOURCES += \
    $$PWD/../../src/cli/*.cpp \
    $$PWD/../../src/CommandLineTool.cpp \
