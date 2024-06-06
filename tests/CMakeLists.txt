include(CTest)
include(FetchContent)

add_executable(netspeak_tests
    netspeak/runner.cpp

	# Add test files here
    netspeak/test_ChainCutter.cpp
    netspeak/test_LfuCache.cpp
    netspeak/test_Netspeak.cpp
    netspeak/test_normalization.cpp
    netspeak/test_parse.cpp
    netspeak/test_phrase.cpp
    netspeak/test_PhraseCorpus.cpp
    netspeak/test_PropertiesFormat.cpp
    netspeak/test_QueryParser.cpp
    netspeak/test_regex.cpp

    netspeak/bighashmap/test_big_hash_map.cpp
    netspeak/bighashmap/test_value_traits.cpp

    netspeak/invertedindex/test_Indexer.cpp
    netspeak/invertedindex/test_InvertedFileReader.cpp
    netspeak/invertedindex/test_ManagedIndexer.cpp
    netspeak/invertedindex/test_Postlist.cpp
    netspeak/invertedindex/test_PostlistReader.cpp
    netspeak/invertedindex/test_PostlistSorter.cpp
    netspeak/invertedindex/test_Record.cpp
)
target_compile_features(netspeak_tests PRIVATE cxx_std_23)
target_link_libraries(netspeak_tests PRIVATE netspeak)

# Boost Test
find_package(Boost 1.40 COMPONENTS unit_test_framework REQUIRED)
target_link_libraries(netspeak_tests PUBLIC ${Boost_LIBRARIES})

# Copy test resources next to the binary
add_custom_command(
    TARGET netspeak_tests PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            ${PROJECT_BINARY_DIR}/data)
            #$<TARGET_FILE_DIR:netspeak_tests>/data)


add_test(NAME netspeak_test COMMAND netspeak_tests)

##########################################################################################
# Code coverage
##########################################################################################
if(CMAKE_COMPILER_IS_GNUCXX)
	include(FetchContent)
	FetchContent_Declare(
		cmakemodules
		GIT_REPOSITORY https://github.com/bilke/cmake-modules.git
		GIT_TAG 1fcf7f4
	)
	FetchContent_MakeAvailable(cmakemodules)
	list(APPEND CMAKE_MODULE_PATH ${cmakemodules_SOURCE_DIR})
    include(CodeCoverage)
    set(GCOV_PATH "gcov-13")
    set(COVERAGE_EXCLUDES "${PROJECT_BINARY_DIR}/**" "/usr/include/**")
    append_coverage_compiler_flags()
    append_coverage_compiler_flags_to_target(netspeak)
    setup_target_for_coverage_lcov(NAME netspeak_coverage EXECUTABLE netspeak_tests)
endif()