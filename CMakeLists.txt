cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

option(NETSPEAK_BUILD_TESTS "Build tests" ON)
option(NETSPEAK_BUILD_DOCS "Build documentation" ON)
option(NETSPEAK_BUILD_DEB "Build debian package" ON)
option(NETSPEAK_ONLY_DOCS "Build only documentation -- this disables tests and others" OFF)

project(Netspeak VERSION 5.0.0 LANGUAGES C CXX)

##########################################################################################
# Enable Optimization
##########################################################################################
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	include(CheckIPOSupported)
	check_ipo_supported(RESULT supported OUTPUT error)
	if( supported )
		message(STATUS "IPO / LTO enabled")
		# set_property(TARGET example PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	else()
		message(STATUS "IPO / LTO not supported: <${error}>")
	endif()
endif()

##########################################################################################
# Source Code
##########################################################################################
if (NOT NETSPEAK_ONLY_DOCS)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	add_subdirectory(src)
	add_subdirectory(bindings/python)
endif()

##########################################################################################
# Testing
##########################################################################################
# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if(NOT NETSPEAK_ONLY_DOCS AND (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND NETSPEAK_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

##########################################################################################
# Documentation
##########################################################################################
# Documentation only available if this is the main app
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND NETSPEAK_BUILD_DOCS)
	find_package(Doxygen REQUIRED)

	## Set Doxygen configuration options (THIS REPLACES THE DOXYFILE!)
	set(DOXYGEN_PROJECT_BRIEF "One word leads to another.")
	set(DOXYGEN_PROJECT_LOGO "netspeak_logo.svg")
	set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/documentation)
	set(DOXYGEN_STRIP_FROM_PATH "\"include\" \"src\"")
	set(DOXYGEN_EXTRACT_PRIVATE "YES")
	set(DOXYGEN_EXTRACT_ALL "YES")
	set(DOXYGEN_EXTRACT_STATIC "YES")
	set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
	set(DOXYGEN_SOURCE_BROWSER "YES")
	set(DOXYGEN_INLINE_SOURCES "YES")
	set(DOXYGEN_RECURSIVE "YES")
	set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/docs/media)
	set(DOXYGEN_HAVE_DOT "YES")  # Required for CALL_GRAPH
	set(DOXYGEN_DOT_IMAGE_FORMAT "svg")
	set(DOXYGEN_CALL_GRAPH "YES")
	set(DOXYGEN_CALLER_GRAPH "YES")
	set(DOXYGEN_MACRO_EXPANSION "YES")
	set(DOXYGEN_UML_LOOK "YES")
	# set(DOXYGEN_DIRECTORY_GRAPH "YES")
	# set(DOXYGEN_DIR_GRAPH_MAX_DEPTH 5)

	set(DOXYGEN_GENERATE_HTML "YES")

    ## Install and configure the "Doxygen Awesome" theme
	include(FetchContent)
	FetchContent_Declare(doxawesome GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css GIT_TAG v2.3.2)
	FetchContent_MakeAvailable(doxawesome)
	set(DOXYGEN_GENERATE_TREEVIEW "YES")
	set(DOXYGEN_DISABLE_INDEX "NO")
	set(DOXYGEN_FULL_SIDEBAR "NO")
	set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxawesome_SOURCE_DIR}/doxygen-awesome.css ${doxawesome_SOURCE_DIR}/doxygen-awesome-sidebar-only.css ${doxawesome_SOURCE_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css ${doxawesome_SOURCE_DIR}/doxygen-awesome-tabs.js ${doxawesome_SOURCE_DIR}/doxygen-custom/custom.css)
	set(DOXYGEN_HTML_COLORSTYLE "LIGHT")
	set(DOXYGEN_HTML_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-header.html)
	set(DOXYGEN_HTML_EXTRA_FILES ${doxawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js ${doxawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js ${doxawesome_SOURCE_DIR}/doxygen-awesome-interactive-toc.js ${doxawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js)

	## Build Documentation
	doxygen_add_docs(
		netspeak_docs
		${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_CURRENT_SOURCE_DIR}/docs
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)

	# Copy Files over that Doxygen has not found (hopefully Doxygen will do this automatically in the future)
	add_custom_command(TARGET netspeak_docs POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/docs/media/netspeak-banner.svg ${DOXYGEN_OUTPUT_DIRECTORY}/html/docs/media/netspeak-banner.svg
	)
endif()

##########################################################################################
# Debian Package
##########################################################################################
# Debian Packaging only available if this is the main app
if(NOT NETSPEAK_ONLY_DOCS AND (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND NETSPEAK_BUILD_DEB)
	install(TARGETS netspeakcli RUNTIME DESTINATION bin)

	SET(CPACK_GENERATOR "DEB")
	SET(CPACK_PACKAGE_NAME netspeakcli)
	SET(CPACK_SET_DESTDIR TRUE)
	SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "info@netspeak.org")
	SET(CPACK_PACKAGE_VERSION "5.0.0rc")
	include(GNUInstallDirs)
	SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Description") # TODO: description
	SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libcmph0, libicu74, libgrpc++1.51t64")
	SET(CPACK_PACKAGE_VENDOR "Webis Group")
	include(CPack)
endif()