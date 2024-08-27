###########################################
## BUILD THE LIBRARY
###########################################
# Include source AND header files here so IDEs can find them
set(PROJECT_HEADERS "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")
add_library(
    ${LIB_NAME} SHARED
    "ComputeDeltaH.cpp"
    "DiffractionLoss.cpp"
    "FindHorizons.cpp"
    "FreeSpaceLoss.cpp"
    "FresnelIntegral.cpp"
    "H0Function.cpp"
    "InitializeArea.cpp"
    "InitializePointToPoint.cpp"
    "InverseComplementaryCumulativeDistributionFunction.cpp"
    "itm_area.cpp"
    "itm_p2p.cpp"
    "KnifeEdgeDiffraction.cpp"
    "LinearLeastSquaresFit.cpp"
    "LineOfSightLoss.cpp"
    "LongleyRice.cpp"
    "QuickPfl.cpp"
    "SigmaHFunction.cpp"
    "SmoothEarthDiffraction.cpp"
    "TerrainRoughness.cpp"
    "TroposcatterLoss.cpp"
    "ValidateInputs.cpp"
    "Variability.cpp"
    "${PROJECT_HEADERS}/Enums.h"
    "${PROJECT_HEADERS}/Errors.h"
    "${PROJECT_HEADERS}/${LIB_NAME}.h"
    "${PROJECT_HEADERS}/Warnings.h"
)

# Add the include directory
target_include_directories(${LIB_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Set minimum C++ version to C++11
target_compile_features(${LIB_NAME} PUBLIC cxx_std_11)

# Platform-specific configurations
if (WIN32)
    set_target_properties(${LIB_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
endif ()

# Set some target metadata
set_target_properties(
    ${LIB_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR} # Include version in .so output filenames
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

# Architecture-dependent configuration
if (BUILD_32BIT)
    set_target_properties(
        ${LIB_NAME} PROPERTIES
        DEBUG_POSTFIX "x86"
        RELEASE_POSTFIX "x86"
    )
else ()
    set_target_properties(
        ${LIB_NAME} PROPERTIES
        DEBUG_POSTFIX "x64"
        RELEASE_POSTFIX "x64"
    )
endif ()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()