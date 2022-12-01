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
target_link_libraries(itmdrvr itmlib)