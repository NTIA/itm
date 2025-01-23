############################################
## CONFIGURE COMMAND LINE DRIVER TESTS
############################################
## TODO-TEMPLATE: Include source AND header files for tests here.
add_executable(
    ${DRIVER_TEST_NAME}
    "TempTextFile.cpp"
    "TestDriver.cpp"
    "TempTextFile.h"
    "TestDriver.h"
    "${DRIVER_HEADERS}/Driver.h"
)

# Add the include directories
target_include_directories(
    ${DRIVER_TEST_NAME} PUBLIC
    "${DRIVER_HEADERS}"
    "${PROJECT_SOURCE_DIR}/app/tests"
)

# Link the library to the executable
target_link_libraries(${DRIVER_TEST_NAME} ${LIB_NAME})

# Set PropLib compiler option defaults
configure_proplib_target(${DRIVER_TEST_NAME})

# Make driver executable location available to source
add_compile_definitions(DRIVER_LOCATION="$<TARGET_FILE:${DRIVER_NAME}>")

###########################################
## SET UP AND DISCOVER TESTS
###########################################
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(${DRIVER_TEST_NAME} ${LIB_NAME} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${DRIVER_TEST_NAME})
