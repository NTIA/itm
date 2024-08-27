# >=3.21 required for Ninja Generators to use absolute paths.
#    See https://stackoverflow.com/questions/69846931/
#    This is relevant for specifying unit test data file paths
#    Automated testing only runs >=3.21 for this reason.
# >=3.14 required for GoogleTest v1.12.x
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

###########################################
## PROJECT METADATA AND CONFIGURATION
###########################################
set(LIB_NAME "ITM")                 # Name of library/target
set(LIB_NAMESPACE "ITS.Propagation")  # Namespace for the named library
project(
    "${LIB_NAMESPACE}.${LIB_NAME}"
    VERSION 1.5.0
    DESCRIPTION "Predicts radiowave propagation for frequencies between 20 MHz and 20 GHz"
    HOMEPAGE_URL "https://github.com/NTIA/itm"
    LANGUAGES "CXX"
)

# Configure wrapper locations
set(DOTNET_WRAPPER_DIR "${PROJECT_SOURCE_DIR}/wrap/dotnet")
set(MATLAB_WRAPPER_DIR "${PROJECT_SOURCE_DIR}/wrap/matlab")
set(PYTHON_WRAPPER_DIR "${PROJECT_SOURCE_DIR}/wrap/python")

# Define options. Defaults to: compile 64-bit library, build docs, run tests
option(BUILD_DOCS "Generate documentation with Doxygen" ON)
option(DOCS_ONLY "Skip all steps except generating documentation" OFF)
option(RUN_TESTS "Run C++ unit tests with Google Test + CTest" ON)
option(COPY_TO_WRAPPERS "Copy compiled library into wrapper submodules" ON)
option(BUILD_32BIT "Build project for x86/32-bit instead of x64/64-bit" OFF)

###########################################
## SETUP
###########################################
# GoogleTest v1.12.1 requires at least C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##########################################
## BUILD/RUN
##########################################
if (NOT DOCS_ONLY)
    add_subdirectory(src)  # Build the shared library
    if (COPY_TO_WRAPPERS)  # Copy compiled library to wrappers
        add_subdirectory(wrap)
    endif ()
    if (RUN_TESTS)         # Build and run unit tests
        if (EXISTS "${PROJECT_SOURCE_DIR}/extern/googletest/CMakeLists.txt")
            enable_testing()
            add_subdirectory(tests)
        else ()
            message(SEND_ERROR
                "Unable to build tests. GoogleTest submodule is missing. "
                "Run `git submodule init extern/googletest` and try again."
            )
        endif()
    endif ()
endif ()

# Generate documentation
if (BUILD_DOCS OR DOCS_ONLY)
    add_subdirectory(docs)
endif ()
