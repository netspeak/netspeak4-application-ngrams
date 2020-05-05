CONFIG += c++11
CONFIG -= qt
DEFINES -= QT_WEBKIT

# For old versions of qmake and gcc (old == Ubuntu 12.04)
QMAKE_CXXFLAGS += -std=c++0x
DEFINES += __GXX_EXPERIMENTAL_CXX0X__

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/../../aitools/aitools3-aq-bighashmap-cpp/src \
    $$PWD/../../aitools/aitools3-aq-invertedindex3-cpp/src \
    $$/usr/include/antlr4-runtime


SOURCES += \
    $$PWD/src/netspeak/request_methods.cpp \
    $$PWD/src/netspeak/query_result_methods.cpp \
    $$PWD/src/netspeak/query_normalization.cpp \
    $$PWD/src/netspeak/query_methods.cpp \
    $$PWD/src/netspeak/Properties.cpp \
    $$PWD/src/netspeak/phrase_methods.cpp \
    $$PWD/src/netspeak/PhraseCorpus.cpp \
    $$PWD/src/netspeak/indexing.cpp \
    $$PWD/src/netspeak/error.cpp \
    $$PWD/src/netspeak/Dictionaries.cpp \
    $$PWD/src/netspeak/Configurations.cpp \
    $$PWD/src/netspeak/generated/NetspeakMessages.pb.cc \
    $$PWD/src/netspeak/regex/DefaultRegexIndex.cpp \
    $$PWD/src/netspeak/regex/parsers.cpp \
    $$PWD/src/netspeak/regex/RegexQuery.cpp \
    $$PWD/src/antlr4/generated/QueryBaseListener.cpp \
    $$PWD/src/antlr4/generated/QueryBaseVisitor.cpp \
    $$PWD/src/antlr4/QueryErrorHandler.cpp \
    $$PWD/src/antlr4/generated/QueryLexer.cpp \
    $$PWD/src/antlr4/generated/QueryListener.cpp \
    $$PWD/src/antlr4/generated/QueryParser.cpp \
    $$PWD/src/antlr4/generated/QueryVisitor.cpp \
    $$PWD/src/antlr4/RawQueryListener.cpp \
    $$PWD/src/antlr4/QueryLengthVerifier.cpp \
    $$PWD/src/antlr4/QueryNormListener.cpp \
    $$PWD/src/antlr4/ProxyListener.cpp \
    $$PWD/src/antlr4/QueryComplexityMeasureListener.cpp

HEADERS += \
    $$PWD/src/netspeak/shell.hpp \
    $$PWD/src/netspeak/typedefs.hpp \
    $$PWD/src/netspeak/RetrievalStrategy3.hpp \
    $$PWD/src/netspeak/RetrievalStrategy2.hpp \
    $$PWD/src/netspeak/RetrievalStrategy1.hpp \
    $$PWD/src/netspeak/RetrievalStrategy.hpp \
    $$PWD/src/netspeak/request_methods.hpp \
    $$PWD/src/netspeak/query_result_methods.hpp \
    $$PWD/src/netspeak/QueryProcessor.hpp \
    $$PWD/src/netspeak/query_normalization.hpp \
    $$PWD/src/netspeak/query_methods.hpp \
    $$PWD/src/netspeak/protobuf_utils.hpp \
    $$PWD/src/netspeak/Properties.hpp \
    $$PWD/src/netspeak/postprocessing.hpp \
    $$PWD/src/netspeak/phrase_methods.hpp \
    $$PWD/src/netspeak/PhraseFileReader.hpp \
    $$PWD/src/netspeak/PhraseFileParser.hpp \
    $$PWD/src/netspeak/PhraseDictionary.hpp \
    $$PWD/src/netspeak/PhraseCorpus.hpp \
    $$PWD/src/netspeak/Netspeak.hpp \
    $$PWD/src/netspeak/LfuCache.hpp \
    $$PWD/src/netspeak/indexing.hpp \
    $$PWD/src/netspeak/error.hpp \
    $$PWD/src/netspeak/Dictionaries.hpp \
    $$PWD/src/netspeak/Configurations.hpp \
    $$PWD/src/netspeak/generated/NetspeakMessages.pb.h \
    $$PWD/src/netspeak/NetspeakRS3.hpp \
    $$PWD/src/antlr4/generated/QueryBaseListener.h \
    $$PWD/src/antlr4/generated/QueryBaseVisitor.h \
    $$PWD/src/antlr4/QueryErrorHandler.hpp \
    $$PWD/src/antlr4/generated/QueryLexer.h \
    $$PWD/src/antlr4/generated/QueryListener.h \
    $$PWD/src/antlr4/generated/QueryParser.h \
    $$PWD/src/antlr4/generated/QueryVisitor.h \
    $$PWD/src/antlr4/RawQueryListener.hpp \
    $$PWD/src/antlr4/QueryLengthVerifier.hpp \
    $$PWD/src/antlr4/QueryNormListener.hpp \
unix: \
    $$PWD/src/antlr4/ProxyListener.hpp \
    $$PWD/src/antlr4/standardMethodsAndValues.hpp \
    $$PWD/src/antlr4/QueryComplexityMeasureListener.hpp
LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lboost_program_options \
    -lboost_regex \
    -lprotobuf \
    -lcmph \
    -lrt \
    -licui18n \
    -licuuc \
    -L$$/usr/lib/ -lantlr4-runtime \

CONFIG(debug) {
  DESTDIR = $$PWD/build/debug
  OBJECTS_DIR = $$PWD/build/debug
}

CONFIG(release, release|debug) {
  DESTDIR = $$PWD/build/release
  OBJECTS_DIR = $$PWD/build/release
  DEFINES += NDEBUG
}
