cmake_minimum_required(VERSION 3.16)
set(UNIT_NAME Core)

set(SOURCES
    LocationManager/LocationManagerBase.cpp
    LocationManager/LocationManagerFactory.cpp
    ConsoleApplication/ConsoleApplication.cpp
    ReportsManager/ReportsManager.cpp
)

set(HEADERS
    LocationManager/LocationManagerBase.h
    LocationManager/LocationManagerFactory.h
    ConsoleApplication/ConsoleApplication.h
    ReportsManager/ReportsManager.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(SOURCES
        ${SOURCES}
        LocationManager/macOS/LocationManagerImpl.mm
        LocationManager/macOS/LocationManagerDelegateBridge.mm
    )

    set(HEADERS
        ${HEADERS}
        LocationManager/macOS/LocationManagerImpl.h
        LocationManager/macOS/LocationManagerDelegateBridge.h
    )

    enable_language(OBJCXX)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "13.3" CACHE STRING "Minimum OS X deployment version")
endif()

add_library(${UNIT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${UNIT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/nod/include
    ${CMAKE_SOURCE_DIR}/src
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(${UNIT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework MapKit"
        "-framework CoreLocation"
    )
endif()