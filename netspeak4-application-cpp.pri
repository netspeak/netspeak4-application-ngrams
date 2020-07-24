CONFIG += c++11
CONFIG -= qt
DEFINES -= QT_WEBKIT
QMAKE_CXX = clang++
QMAKE_LINK = clang++
QMAKE_CXXFLAGS += -Wall -Wextra #-g -O0
#QMAKE_LFLAGS += -g -O0 -rdynamic

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/build/dependencies/aitools3-aq-bighashmap-cpp/src \
    $$PWD/build/dependencies/aitools3-aq-invertedindex3-cpp/src \
    $$/usr/include/antlr4-runtime


SOURCES += \
    $$PWD/src/antlr4/*.cpp \
    $$PWD/src/antlr4/generated/*.cpp \
    $$PWD/src/netspeak/*.cpp \
    $$PWD/src/netspeak/internal/*.cpp \
    $$PWD/src/netspeak/regex/*.cpp \
    $$PWD/src/netspeak/service/*.cc \

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
    -l:libantlr4-runtime.a \
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
