###########################################
## BUILD THE LIBRARY
###########################################
proplib_message("Configuring library ${LIB_NAME}")

set(LIB_HEADERS "${PROJECT_SOURCE_DIR}/include")
set(LIB_FILES
    "ReturnCodes.cpp"
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
    "${LIB_HEADERS}/${LIB_NAME}.h"
)

# By default, create shared library
if (NOT DEFINED BUILD_SHARED_LIBS)
    message(STATUS "STATUS: BUILD_SHARED_LIBS is not defined to build the library: " ${LIB_NAME} ".")
    add_library(${LIB_NAME} SHARED ${LIB_FILES})
else ()
    message(STATUS "STATUS: BUILD_SHARED_LIBS is " ${BUILD_SHARED_LIBS} " to build the library: " ${LIB_NAME} ".")
    add_library(${LIB_NAME} ${LIB_FILES})
endif ()

# Add the include directory
target_include_directories(${LIB_NAME} PUBLIC "${LIB_HEADERS}")

# Set PropLib compiler option defaults
configure_proplib_target(${LIB_NAME})

# Add definition to get the library name and version inside the library
add_compile_definitions(
    LIBRARY_NAME="${LIB_NAME}"
    LIBRARY_VERSION="${PROJECT_VERSION}"
)

# Platform-specific configurations
if (WIN32)
    set_target_properties(${LIB_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
endif ()
if (UNIX)
    # avoid prefixing "lib" to the output file, for cross-platform consistency
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

# Set some target metadata
set_target_properties(
    ${LIB_NAME} PROPERTIES
    OUTPUT_NAME ${LIB_NAME}-${PROJECT_VERSION}-
    VERSION ${PROJECT_VERSION}
    DEBUG_POSTFIX ${ARCH_SUFFIX}
    RELEASE_POSTFIX ${ARCH_SUFFIX}
)

proplib_message("Done configuring library ${LIB_NAME}")
