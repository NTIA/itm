#
# Cmake for a shared library using directions at https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
#
cmake_minimum_required(VERSION 3.16)

project(itmlib VERSION 1.3 DESCRIPTION "NTIA Irregular Terrain Model library")

SET(itmlib_SRC
    src/ComputeDeltaH.cpp
    src/DiffractionLoss.cpp
    src/FindHorizons.cpp
    src/FreeSpaceLoss.cpp
    src/FresnelIntegral.cpp
    src/H0Function.cpp
    src/InitializeArea.cpp
    src/InitializePointToPoint.cpp
    src/InverseComplementaryCumulativeDistributionFunction.cpp
    src/itm_area.cpp
    src/itm_p2p.cpp
    src/KnifeEdgeDiffraction.cpp
    src/LinearLeastSquaresFit.cpp
    src/LineOfSightLoss.cpp
    src/LongleyRice.cpp
    src/QuickPfl.cpp
    src/SigmaHFunction.cpp
    src/SmoothEarthDiffraction.cpp
    src/TerrainRoughness.cpp
    src/TroposcatterLoss.cpp
    src/ValidateInputs.cpp
    src/Variability.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

#
# run cmake -DBUILD_SHARED_LIBS=ON if you want shared libs
# See https://cgold.readthedocs.io/en/latest/tutorials/libraries/static-shared.html
#
add_library(itmlib ${itmlib_SRC})

set_target_properties(itmlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(itmlib PROPERTIES SOVERSION 0)
add_compile_definitions(
    ITMLIB_VERSION="${PROJECT_VERSION}.0")

# Use C++14 -- needed because of initializers
set_property(TARGET itmlib PROPERTY CXX_STANDARD 14)

set_target_properties(itmlib PROPERTIES PUBLIC_HEADER include/itm.h)
set_target_properties(itmlib PROPERTIES PUBLIC_HEADER include/Enums.h)
set_target_properties(itmlib PROPERTIES PUBLIC_HEADER include/Errors.h)
set_target_properties(itmlib PROPERTIES PUBLIC_HEADER include/Warnigns.h)

# Only Cmake 3.20+ provides platform independent way to construct paths
IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER_EQUAL 3.20)
    cmake_path(APPEND itminclude "${CMAKE_SOURCE_DIR}" "include")
ELSE (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER_EQUAL 3.20)
    set(itminclude "${CMAKE_SOURCE_DIR}/include")
ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER_EQUAL 3.20)

#
# inclue the include directory
#
target_include_directories(itmlib PRIVATE ${itminclude})

include(GNUInstallDirs)
install(TARGETS itmlib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/itmlib
    )

# pkg_check_modules()

configure_file(itmlib.pc.in itmlib.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/itmlib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_subdirectory(win32/ITMDrvr)
