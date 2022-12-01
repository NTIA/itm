cmake_minimum_required(VERSION 3.9)

SET(ITMDRVR_SRC
    AreaMode.cpp
    Common.cpp 
    ITMDrvr.cpp 
    PointToPointMode.cpp 
    Reporting.cpp 
)


add_executable(itmdrvr ${ITMDRVR_SRC})
target_include_directories(itmdrvr PRIVATE 
    ${itminclude}
)

# Use C++14
set_property(TARGET itmdrvr PROPERTY CXX_STANDARD 14)

target_link_libraries(itmdrvr itmlib)
