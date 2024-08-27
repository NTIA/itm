###########################################
## COPY COMPILED LIBRARY TO WRAPPERS
###########################################
# Wrapper directories are configured as variables
# in the top-level CMakeLists.txt file. This file
# checks if `CMakeLists.txt` exists in each wrapper
# directory, and if so, adds that subdirectory.

# C#/.NET
if (EXISTS "${DOTNET_WRAPPER_DIR}/CMakeLists.txt")
    add_subdirectory(${DOTNET_WRAPPER_DIR})
else ()
    message(STATUS "Skipping copying compiled library to C#/.NET wrapper: submodule not initialized.")
endif ()

# MATLAB
if (EXISTS "${MATLAB_WRAPPER_DIR}/CMakeLists.txt")
    add_subdirectory(${MATLAB_WRAPPER_DIR})
else ()
    message(STATUS "Skipping copying compiled library to MATLAB wrapper: submodule not initialized")
endif ()

# Python
if (EXISTS "${PYTHON_WRAPPER_DIR}/CMakeLists.txt")
    add_subdirectory(${PYTHON_WRAPPER_DIR})
else ()
    message(STATUS "Skipping copying compiled library to Python wrapper: submodule not initialized")
endif ()