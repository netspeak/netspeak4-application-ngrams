include(FetchContent)
include(FindPkgConfig)

add_library(netspeak)
target_sources(netspeak PRIVATE
    antlr4/parse.cpp
    antlr4/QueryErrorHandler.cpp

    cli/BuildCommand.cpp
    cli/Commands.cpp
    cli/logging.cpp
    cli/ProxyCommand.cpp
    cli/RegexCommand.cpp
    cli/ServeCommand.cpp
    cli/ShellCommand.cpp
    cli/StressCommand.cpp
    cli/util.cpp

    netspeak/Configuration.cpp
    netspeak/Dictionaries.cpp
    netspeak/error.cpp
    netspeak/indexing.cpp
    netspeak/Netspeak.cpp
    netspeak/PhraseCorpus.cpp
    netspeak/Properties.cpp
    netspeak/QueryNormalizer.cpp

    netspeak/invertedindex/ByteBuffer.cpp
    netspeak/invertedindex/Properties.cpp

    netspeak/model/LengthRange.cpp
    netspeak/model/NormQuery.cpp
    netspeak/model/Phrase.cpp
    netspeak/model/Query.cpp
    netspeak/model/RawRefResult.cpp
    netspeak/model/RawResult.cpp
    netspeak/model/SimpleQuery.cpp
    netspeak/model/Words.cpp

    netspeak/regex/DefaultRegexIndex.cpp
    netspeak/regex/parsers.cpp
    netspeak/regex/RegexQuery.cpp

    netspeak/service/LoadBalanceProxy.cpp
    netspeak/service/RequestLogger.cpp
    netspeak/service/tracking.cpp
    netspeak/service/UniqueMap.cpp

    netspeak/util/Config.cpp
    netspeak/util/FileDescriptor.cpp
    netspeak/util/glob.cpp
    netspeak/util/logging.cpp
    netspeak/util/PropertiesFormat.cpp
    netspeak/util/service.cpp
    netspeak/util/string.cpp
)
target_compile_features(netspeak PUBLIC cxx_std_23)
target_include_directories(netspeak PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)
set_target_properties(netspeak PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(netspeakcli)
target_sources(netspeakcli PRIVATE
    CommandLineTool.cpp
)

target_compile_features(netspeakcli PUBLIC cxx_std_23)
target_link_libraries(netspeakcli PUBLIC netspeak)

# We want to build everything into a single binary
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
if (WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()
if (MSVC)
    target_compile_options(netspeakcli PUBLIC /MT)
    target_link_options(netspeakcli PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
else()
    # target_link_libraries(netspeakcli PUBLIC "-static")
endif()

##########################################################################################
# Libraries
##########################################################################################
# ANTLR4
set(ANTLR_VERSION 4.13.1)
# Download the jar file
FetchContent_Declare(antlrjar URL https://www.antlr.org/download/antlr-${ANTLR_VERSION}-complete.jar DOWNLOAD_NO_EXTRACT TRUE)
FetchContent_MakeAvailable(antlrjar)
set(ANTLR_EXECUTABLE ${antlrjar_SOURCE_DIR}/antlr-${ANTLR_VERSION}-complete.jar)
# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)
# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR_BUILD_CPP_TESTS OFF)
FetchContent_Declare(
    antlr
    GIT_REPOSITORY https://github.com/antlr/antlr4
    GIT_TAG ${ANTLR_VERSION}
    SOURCE_SUBDIR "runtime/Cpp"
)
FetchContent_MakeAvailable(antlr)
set(ANTLR4CPP_INCLUDE_DIRS ${antlr_SOURCE_DIR}/runtime/Cpp/runtime/src)
include(${antlr_SOURCE_DIR}/runtime/Cpp/cmake/FindANTLR.cmake)

antlr_target(nsgrammarparser ${CMAKE_CURRENT_LIST_DIR}/../conf/grammar/Query.g4 LEXER PARSER VISITOR LISTENER PACKAGE antlr4)

target_link_libraries(netspeak PUBLIC antlr4_static)
target_include_directories(netspeak PUBLIC ${ANTLR4CPP_INCLUDE_DIRS} ${ANTLR_nsgrammarparser_OUTPUT_DIR})
target_sources(netspeak PRIVATE ${ANTLR_nsgrammarparser_CXX_OUTPUTS})

# absl (needed by protobuf)
FetchContent_Declare(absl GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git GIT_TAG 20240116.2 OVERRIDE_FIND_PACKAGE)
set(ABSL_ENABLE_INSTALL ON)  # https://github.com/protocolbuffers/protobuf/issues/12185#issuecomment-1594685860
FetchContent_MakeAvailable(absl)


# gRPC
find_package(gRPC CONFIG)
if (gRPC_FOUND)
    set(GRPC_PLUGIN_TARGET $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
    set(GRPC_LIBRARIES gRPC::grpc++)
else()
    FetchContent_Declare(gRPC GIT_REPOSITORY https://github.com/grpc/grpc GIT_TAG v1.62.1)
    set(gRPC_SSL_PROVIDER package)
    # set(gRPC_PROTOBUF_PROVIDER package)
    FetchContent_MakeAvailable(gRPC)
    set(GRPC_PLUGIN_TARGET $<TARGET_FILE:grpc_cpp_plugin>)
    set(GRPC_LIBRARIES grpc++)
endif()

target_link_libraries(netspeak PUBLIC ${GRPC_LIBRARIES})

# Protobuf and gRPC sources
find_package(Protobuf REQUIRED)
target_link_libraries(netspeak PUBLIC ${Protobuf_LIBRARIES})
set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(netspeak PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

target_sources(netspeak PRIVATE netspeak/NetspeakService.proto)
protobuf_generate(TARGET netspeak)
protobuf_generate(
    TARGET netspeak
    LANGUAGE grpc
    PLUGIN protoc-gen-grpc=${GRPC_PLUGIN_TARGET}
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
)
# target_sources(netspeak PUBLIC ${NS_PROTO_CPP})
# target_include_directories(netspeak PUBLIC ${NS_PROTO_HPP})

# Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.40 COMPONENTS program_options regex REQUIRED)
target_link_libraries(netspeak PUBLIC ${Boost_LIBRARIES})

# cmph
pkg_check_modules(CMPH REQUIRED cmph)
target_link_libraries(netspeak PUBLIC cmph)

# ICU
find_package(ICU COMPONENTS uc data REQUIRED) # Component-order is important! https://stackoverflow.com/a/76405185
target_link_libraries(netspeak PUBLIC ${ICU_LIBRARIES})
