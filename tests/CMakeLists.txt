############################################
## CONFIGURE UNIT TESTS
############################################
set(TEST_NAME "${LIB_NAME}Test")
proplib_message("Configuring library tests ${TEST_NAME}")

add_executable(
    ${TEST_NAME}
    "TestComputeDeltaH.cpp"
    "TestDiffractionLoss.cpp"
    "TestEquivalence.cpp"
    "TestFindHorizons.cpp"
    "TestFreeSpaceLoss.cpp"
    "TestFresnelIntegral.cpp"
    "TestH0Function.cpp"
    "TestInitializeArea.cpp"
    "TestInitializePointToPoint.cpp"
    "TestInverseComplementaryCumulativeDistributionFunction.cpp"
    "TestKnifeEdgeDiffraction.cpp"
    "TestLinearLeastSquaresFit.cpp"
    "TestLineOfSightLoss.cpp"
    "TestLongleyRice.cpp"
    "TestQuickPfl.cpp"
    "TestSigmaHFunction.cpp"
    "TestSmoothEarthDiffraction.cpp" 
    "TestTerrainRoughness.cpp"
    "TestTroposcatterLoss.cpp"
    "TestValidateInputs.cpp"
    "TestVariability.cpp"
    "TestUtils.cpp"
    "TestUtils.h"
)

# Set PropLib compiler option defaults
configure_proplib_target(${TEST_NAME})

###########################################
## SET UP AND DISCOVER TESTS
###########################################
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(${TEST_NAME} ${LIB_NAME} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})

proplib_message("Done configuring library tests ${TEST_NAME}")
