CONFIG += c++11
CONFIG -= qt
DEFINES -= QT_WEBKIT
QMAKE_CXX = clang++
QMAKE_LINK = clang++
QMAKE_CXXFLAGS += -Wall -Wextra -g -ggdb -O0
QMAKE_LFLAGS += -g -O0 -rdynamic

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/build/dependencies/aitools3-aq-bighashmap-cpp/src \
    $$PWD/build/dependencies/aitools3-aq-invertedindex3-cpp/src \
    $$/usr/include/antlr4-runtime


SOURCES += \
    $$PWD/src/antlr4/generated/QueryBaseListener.cpp \
    $$PWD/src/antlr4/generated/QueryBaseVisitor.cpp \
    $$PWD/src/antlr4/generated/QueryLexer.cpp \
    $$PWD/src/antlr4/generated/QueryListener.cpp \
    $$PWD/src/antlr4/generated/QueryParser.cpp \
    $$PWD/src/antlr4/generated/QueryVisitor.cpp \
    $$PWD/src/antlr4/parse.cpp \
    $$PWD/src/antlr4/QueryErrorHandler.cpp \
    $$PWD/src/netspeak/Configurations.cpp \
    $$PWD/src/netspeak/Dictionaries.cpp \
    $$PWD/src/netspeak/error.cpp \
    $$PWD/src/netspeak/indexing.cpp \
    $$PWD/src/netspeak/Netspeak.cpp \
    $$PWD/src/netspeak/PhraseCorpus.cpp \
    $$PWD/src/netspeak/Properties.cpp \
    $$PWD/src/netspeak/QueryNormalizer.cpp \
    $$PWD/src/netspeak/internal/LengthRange.cpp \
    $$PWD/src/netspeak/internal/NormQuery.cpp \
    $$PWD/src/netspeak/internal/Phrase.cpp \
    $$PWD/src/netspeak/internal/Query.cpp \
    $$PWD/src/netspeak/internal/RawRefResult.cpp \
    $$PWD/src/netspeak/internal/RawResult.cpp \
    $$PWD/src/netspeak/internal/SimpleQuery.cpp \
    $$PWD/src/netspeak/internal/Words.cpp \
    $$PWD/src/netspeak/regex/DefaultRegexIndex.cpp \
    $$PWD/src/netspeak/regex/parsers.cpp \
    $$PWD/src/netspeak/regex/RegexQuery.cpp \
    $$PWD/src/netspeak/service/NetspeakService.grpc.pb.cc \
    $$PWD/src/netspeak/service/NetspeakService.pb.cc \

HEADERS += \
    $$PWD/src/netspeak/shell.hpp \
    $$PWD/src/netspeak/RetrievalStrategy3.hpp \
    $$PWD/src/netspeak/RetrievalStrategy.hpp \
    $$PWD/src/netspeak/QueryProcessor.hpp \
    $$PWD/src/netspeak/Properties.hpp \
    $$PWD/src/netspeak/postprocessing.hpp \
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

LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lboost_program_options \
    -lboost_regex \
    -lboost_stacktrace_basic \
    -lboost_stacktrace_backtrace \
    -lboost_stacktrace_addr2line \
    -ldl \
    -lz \
    -lcmph \
    -lrt \
    -lpthread \
    -licui18n \
    -licuuc \
    -L$$/usr/lib/ \
    -lantlr4-runtime \
    -l:libprotobuf.a \
    -l:libgrpc++.a \
    -l:libgrpc_unsecure.a \
    -l:libcares.a \
    -l:libgrpc.a \
    -l:libgpr.a \
    -l:libabsl_bad_optional_access.a \
    -l:libabsl_base.a \
    -l:libabsl_log_severity.a \
    -l:libabsl_spinlock_wait.a \
    -l:libabsl_str_format_internal.a \
    -l:libabsl_strings.a \
    -l:libabsl_raw_logging_internal.a \
    -l:libabsl_strings_internal.a \
    -l:libabsl_int128.a \
    -l:libabsl_throw_delegate.a \
    -l:libaddress_sorting.a \
    -l:libupb.a \

CONFIG(debug) {
  DESTDIR = $$PWD/build/debug
  OBJECTS_DIR = $$PWD/build/debug
}

CONFIG(release, release|debug) {
  DESTDIR = $$PWD/build/release
  OBJECTS_DIR = $$PWD/build/release
  #DEFINES += NDEBUG
}
