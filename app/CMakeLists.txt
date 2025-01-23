###########################################
## CONFIGURE THE COMMAND LINE DRIVER
###########################################
set(DRIVER_NAME "${LIB_NAME}Driver")
set(DRIVER_VERSION ${PROJECT_VERSION}.0)
set(DRIVER_HEADERS "${PROJECT_SOURCE_DIR}/app/include")
set(DRIVER_TEST_NAME "${DRIVER_NAME}Test")

###########################################
## BUILD THE COMMAND LINE DRIVER
###########################################
proplib_message("Configuring command line driver ${DRIVER_NAME}")
add_subdirectory(src)
proplib_message("Done configuring command line driver ${DRIVER_NAME}")

###########################################
## BUILD AND RUN THE DRIVER TESTS
###########################################
if (RUN_DRIVER_TESTS)
    proplib_message("Configuring command line driver tests ${DRIVER_TEST_NAME}")
    set_target_properties(${DRIVER_NAME} PROPERTIES ENABLE_EXPORTS ON)
    add_subdirectory(tests)
    proplib_message("Done configuring command line driver tests ${DRIVER_TEST_NAME}")
endif()
