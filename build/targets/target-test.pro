TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
TARGET = netspeak4-test



COMMON_INCLUDE = netspeak4-application-cpp.pri
!include(../../$$COMMON_INCLUDE) {
  error("Could not find $$COMMON_INCLUDE file")
}

SOURCES += \
    ../../test/netspeak/runner.cpp \
    ../../test/netspeak/test_ChainCutter.cpp \
    ../../test/netspeak/test_LfuCache.cpp \
    ../../test/netspeak/test_Netspeak.cpp \
    ../../test/netspeak/test_PhraseCorpus.cpp \
    ../../test/netspeak/test_phrase.cpp \
    ../../test/netspeak/test_QueryParser.cpp \
    ../../test/netspeak/test_normalization.cpp \
    ../../test/netspeak/test_regex.cpp \
    ../../test/netspeak/test_parse.cpp \

HEADERS += \
    ../../test/netspeak/paths.hpp \
