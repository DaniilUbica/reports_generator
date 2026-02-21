cmake_minimum_required(VERSION 3.16)
set(UNIT_NAME GUI)

set(SOURCES
    MapView/MapViewBase.cpp
    Application/Application.cpp
    AddressInputManager/AddressInputManager.cpp
)

set(HEADERS
    MapView/MapViewBase.h
    MapView/MapViewRegister.hpp
    Application/Application.h
    AddressInputManager/AddressInputManager.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(SOURCES
        ${SOURCES}
        MapView/macOS/MapViewImpl.mm
        MapView/macOS/MapViewDelegateBridge.mm

        ${CMAKE_SOURCE_DIR}/bundle/Info.plist
    )

    set(HEADERS
        ${HEADERS}

        MapView/macOS/MapViewImpl.h
        MapView/macOS/MapViewDelegateBridge.h
    )

    enable_language(OBJCXX)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "13.3" CACHE STRING "Minimum OS X deployment version")
endif()

add_library(${UNIT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(${UNIT_NAME} PRIVATE 
    Qt6::Core
    Qt6::Quick

    Core
)

target_include_directories(${UNIT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(${UNIT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework MapKit"
        "-framework CoreLocation"
    )
endif()

set_target_properties(${UNIT_NAME} PROPERTIES
    AUTOMOC ON
)
