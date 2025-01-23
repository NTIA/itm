############################################################
## CMakeList.txt : Top-level CMake project file, do global
## configuration and include sub-projects here.
############################################################

# >=3.21 required for Ninja Generators to use absolute paths.
#    See https://stackoverflow.com/questions/69846931/
#    This is relevant for specifying unit test data file paths
#    Automated testing only runs >=3.21 for this reason.
# >=3.15 required for CMake features used in this project
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Warn if '-A Win32' is provided but BUILD_32BIT has not been set. This may
# indicate accidental use of a 64-bit CMake preset while intending to build for 32-bit.
if (DEFINED ENV{CMAKE_GENERATOR_PLATFORM})
    if ($ENV{CMAKE_GENERATOR_PLATFORM} STREQUAL "Win32")
        if (NOT BUILD_32BIT)
            message(WARNING "Generator platform is Win32 but 32-bit preset is not being used.")
        endif ()
    endif ()
endif ()

# If on macOS, handle arm64/x86_64 architectures (must be done before project())
# For other OS, identify the architecture and save it to the ARCH_SUFFIX variable.
if (APPLE)
    # Get the current platform's native architecture
    execute_process(
        COMMAND uname -m
        RESULT_VARIABLE result
        OUTPUT_VARIABLE MACOS_NATIVE_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # If running on Apple silicon, try a universal build. Otherwise, a native build.
    if (${MACOS_NATIVE_ARCHITECTURE} STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
        set(ARCH_SUFFIX "universal")
    else ()
        set(ARCH_SUFFIX ${MACOS_NATIVE_ARCHITECTURE})
    endif ()
elseif (WIN32)
    if (BUILD_32BIT)
        set(ARCH_SUFFIX "x86")
    else ()
        set(ARCH_SUFFIX "x64")
    endif ()
elseif (UNIX) # Only non-Apple Linux evaluates as True here
    set(ARCH_SUFFIX "x86_64")
endif ()

# Convenience function for printing visible status messages.
# Accepts arbitrary number of string inputs, each printed as a 
# new line in the status message.
function(proplib_message)
    string(REPLACE ";" "\n--   " all_args "${ARGN}")
    message(STATUS 
    "==========[PROPLIB STATUS MESSAGE]==========\n"
    "--   ${all_args}\n-- "
    "============================================\n"
    )
endfunction()

###########################################
## PROJECT METADATA
###########################################
set(LIB_NAME "ITM")                   # Name of library/target
set(LIB_NAMESPACE "ITS.Propagation")  # Namespace for the named library
project(
    "${LIB_NAMESPACE}.${LIB_NAME}"
    VERSION 1.5
    DESCRIPTION "Predicts radiowave propagation for frequencies between 20 MHz and 20 GHz"
    HOMEPAGE_URL "https://ntia.github.io/propagation-library-wiki/models/ITM"
    LANGUAGES "CXX"
)

###########################################
## CMAKE OPTIONS AND DEFAULTS
###########################################
# Define options. Defaults to: compile 64-bit library and driver, build docs, run tests
option(BUILD_DOCS "Generate documentation site with Doxygen" ON)
option(BUILD_DRIVER "Build the command-line driver executable" ON)
option(RUN_DRIVER_TESTS "Test the command-line driver executable" ON)
option(DOCS_ONLY "Skip all steps except generating the documentation site" OFF)
option(RUN_TESTS "Run unit tests for the main library" ON)
option(BUILD_32BIT "Build project for x86/32-bit instead of x64/64-bit" OFF)

###########################################
## SETUP
###########################################
# C++11 and some extensions (e.g., for `auto`) are the minimum required
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Get 0/1 values indicating compiler type
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Define a function to set compile options of a provided target.
function(configure_proplib_target proplib_target)
    target_compile_options(${proplib_target} PRIVATE
        # For GCC-like compilers in any configuration
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-static>>"
        # For GCC-like compilers in Release configurations
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:$<$<CONFIG:Release>:-O3;-DNDEBUG>>>"
        # For GCC-like compilers in Debug configurations
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:$<$<CONFIG:Debug>:-g;-O0>>>"
        # For GCC-like compilers in 32-bit configurations
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:$<$<BOOL:${BUILD_32BIT}>:-m32>>>"
        # For MSVC compiler in any configuration
        "$<${msvc_cxx}:$<BUILD_INTERFACE:/Gd>>"
        # For MSVC compiler in Release configurations
        "$<${msvc_cxx}:$<BUILD_INTERFACE:$<$<CONFIG:Release>:/W3;/O2;/DNDEBUG>>>"
        # For MSVC compiler in Debug configurations
        "$<${msvc_cxx}:$<BUILD_INTERFACE:$<$<CONFIG:Debug>:/W4;/Od;/Zi>>>"
    )
endfunction()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

# control where the static and shared libraries are built
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin" CACHE STRING "Set the CMAKE Archive Output Directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin" CACHE STRING "Set the CMAKE Library Output Directory")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin" CACHE STRING "Set the CMAKE Runtime Output Directory")

proplib_message(
    "Initial Configuration Complete"
    "Generator: ${CMAKE_GENERATOR}"
    "Generator platform (for multi-config generators only): $ENV{CMAKE_GENERATOR_PLATFORM}"
    "C++ Compiler: ${CMAKE_CXX_COMPILER}"
    "Target architecture: ${ARCH_SUFFIX}"
    "CMake Options:"
    "  BUILD_32BIT = ${BUILD_32BIT}"
    "  BUILD_DOCS = ${BUILD_DOCS}"
    "  BUILD_DRIVER = ${BUILD_DRIVER}"
    "  RUN_DRIVER_TESTS = ${RUN_DRIVER_TESTS}"
    "  DOCS_ONLY = ${DOCS_ONLY}"
    "  RUN_TESTS = ${RUN_TESTS}"
)

##########################################
## BUILD/RUN
##########################################
if (NOT DOCS_ONLY)
    add_subdirectory(src)  # Configure the library
    if (RUN_TESTS OR RUN_DRIVER_TESTS)
        enable_testing()
        include(CTest)
        # Initialize GoogleTest if any tests will be configured
        if (EXISTS "${PROJECT_SOURCE_DIR}/extern/googletest/CMakeLists.txt")
            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
            # Ensure GoogleTest is built as a static library
            if (DEFINED BUILD_SHARED_LIBS AND BUILD_SHARED_LIBS)
                set(BUILD_SHARED_LIBS_${LIB_NAME} ${BUILD_SHARED_LIBS})
                set(BUILD_SHARED_LIBS OFF)
            endif ()
            add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest" EXCLUDE_FROM_ALL)
            include(GoogleTest)
            # Restore initial value of BUILD_SHARED_LIBS
            if (DEFINED BUILD_SHARED_LIBS_${LIB_NAME})
                set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_${LIB_NAME}})
            endif ()
        else ()
            message(SEND_ERROR
                "Unable to build tests. GoogleTest submodule is missing. "
                "Run `git submodule init extern/googletest` then "
                "`git submodule update` and try again."
            )
        endif ()
    endif ()
    if (RUN_TESTS)         # Build and run unit tests
        add_subdirectory(tests)
    endif ()
    if (BUILD_DRIVER OR RUN_DRIVER_TESTS)
        add_subdirectory(app)
    endif ()
endif ()

# Generate documentation
if (BUILD_DOCS OR DOCS_ONLY)
    add_subdirectory(docs)
endif ()
