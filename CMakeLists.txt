cmake_minimum_required(VERSION 3.14)
project(netspeak4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-m64")
set(CMAKE_EXE_LINKER_FLAGS "-m64")


SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

#set(CMAKE_CXX_CPPCHECK "cppcheck")
#list(APPEND CMAKE_CXX_CPPCHECK
#            --enable=all -isrc/antlr4/generated/** -i/usr/include/**)



include_directories(
    "${PROJECT_BINARY_DIR}/src"
    "${PROJECT_BINARY_DIR}/build/dependencies/aitools3-aq-bighashmap-cpp/src"
    "${PROJECT_BINARY_DIR}/build/dependencies/aitools3-aq-invertedindex3-cpp/src"
    "/usr/include/antlr4-runtime"
)

add_executable(netspeak4 src/CommandLineTool.cpp)
target_sources(netspeak4 PRIVATE
"src/antlr4/parse"
"src/antlr4/QueryErrorHandler"

"src/antlr4/generated/QueryBaseListener"
"src/antlr4/generated/QueryBaseVisitor"
"src/antlr4/generated/QueryLexer"
"src/antlr4/generated/QueryListener"
"src/antlr4/generated/QueryParser"
"src/antlr4/generated/QueryVisitor"

"src/cli/Commands"
"src/cli/BuildCommand"
"src/cli/ProxyCommand"
"src/cli/RegexCommand"
"src/cli/ServeCommand"
"src/cli/ShellCommand"
"src/cli/util"

"src/netspeak/Configuration"
"src/netspeak/Dictionaries"
"src/netspeak/error"
"src/netspeak/indexing"
"src/netspeak/Netspeak"
"src/netspeak/PhraseCorpus"
"src/netspeak/PhraseDictionary"
"src/netspeak/PhraseFileParser"
"src/netspeak/PhraseFileReader"
"src/netspeak/Properties"
"src/netspeak/QueryNormalizer"
"src/netspeak/QueryProcessor"
"src/netspeak/RetrievalStrategy"
"src/netspeak/RetrievalStrategy3"

"src/netspeak/internal/LengthRange"
"src/netspeak/internal/NormQuery"
"src/netspeak/internal/Phrase"
"src/netspeak/internal/Query"
"src/netspeak/internal/QuerySyntax"
"src/netspeak/internal/RawPhraseResult"
"src/netspeak/internal/RawRefResult"
"src/netspeak/internal/RawResult"
"src/netspeak/internal/SearchOptions"
"src/netspeak/internal/SearchResult"
"src/netspeak/internal/SimpleQuery"
"src/netspeak/internal/typedefs"
"src/netspeak/internal/Words"

"src/netspeak/regex/DefaultRegexIndex"
"src/netspeak/regex/parsers"
"src/netspeak/regex/RegexIndex"
"src/netspeak/regex/RegexQuery"

"src/netspeak/service/LoadBalanceProxy"
"src/netspeak/service/NetspeakService.grpc.pb"
"src/netspeak/service/NetspeakService.pb"
"src/netspeak/service/UniqueMap"

"src/netspeak/util/ChainCutter"
"src/netspeak/util/Config"
"src/netspeak/util/FileDescriptor"
"src/netspeak/util/glob"
"src/netspeak/util/LfuCache"
"src/netspeak/util/Math"
"src/netspeak/util/PropertiesFormat"
"src/netspeak/util/service"
"src/netspeak/util/traceable_error"
"src/netspeak/util/Vec"
)

target_link_libraries(netspeak4
dl
z
cmph
rt
pthread
icui18n
icuuc

# boost
boost_filesystem
boost_system
boost_program_options
boost_regex
boost_stacktrace_basic
boost_stacktrace_backtrace
boost_stacktrace_addr2line

# maybe?
-L/usr/lib/

# STATIC
-l:libantlr4-runtime.a
-l:libprotobuf.a
-l:libgrpc++.a
-l:libgrpc_unsecure.a
-l:libcares.a
-l:libgrpc.a
-l:libgpr.a
-l:libabsl_bad_optional_access.a
-l:libabsl_base.a
-l:libabsl_log_severity.a
-l:libabsl_spinlock_wait.a
-l:libabsl_str_format_internal.a
-l:libabsl_strings.a
-l:libabsl_raw_logging_internal.a
-l:libabsl_strings_internal.a
-l:libabsl_int128.a
-l:libabsl_throw_delegate.a
-l:libaddress_sorting.a
-l:libupb.a
)
