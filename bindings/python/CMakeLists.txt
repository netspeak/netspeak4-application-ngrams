FetchContent_Declare(pybind11 GIT_REPOSITORY https://github.com/pybind/pybind11.git GIT_TAG v2.12.0)
FetchContent_MakeAvailable(pybind11)

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../build/dependencies/pybind11-2.9.0)
pybind11_add_module(netspeak4py
    src/PythonBindings.cpp
    src/py/QueryParser.cpp
)
set_property(TARGET netspeak4py PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(netspeak4py PRIVATE pybind11::module netspeak)