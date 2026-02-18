cmake_minimum_required(VERSION 3.16)
project(ReportsGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    main.cpp

    ${SOURCES_DIR}/MapView/MapViewBase.cpp
    ${SOURCES_DIR}/LocationManager/LocationManagerBase.cpp
    ${SOURCES_DIR}/LocationManager/LocationManagerFactory.cpp

    qml.qrc
)

set(HEADERS
    ${SOURCES_DIR}/MapView/MapViewBase.h
    ${SOURCES_DIR}/MapView/MapViewRegister.hpp
    ${SOURCES_DIR}/LocationManager/LocationManagerBase.h
    ${SOURCES_DIR}/LocationManager/LocationManagerFactory.h
)

set(RESOURCE
    qml.qrc
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(SOURCES
        ${SOURCES}
        ${SOURCES_DIR}/MapView/macOS/MapViewImpl.mm
        ${SOURCES_DIR}/MapView/macOS/MapViewDelegateBridge.mm
        ${SOURCES_DIR}/LocationManager/macOS/LocationManagerImpl.mm
        ${SOURCES_DIR}/LocationManager/macOS/LocationManagerDelegateBridge.mm

        ${CMAKE_CURRENT_SOURCE_DIR}/bundle/Info.plist
    )

    set(HEADERS
        ${HEADERS}

        ${SOURCES_DIR}/MapView/macOS/MapViewImpl.h
        ${SOURCES_DIR}/MapView/macOS/MapViewDelegateBridge.h
        ${SOURCES_DIR}/LocationManager/macOS/LocationManagerImpl.h
        ${SOURCES_DIR}/LocationManager/macOS/LocationManagerDelegateBridge.h
    )

    enable_language(OBJCXX)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
endif()

add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${RESOURCE}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${SOURCES_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick

    "-framework Cocoa"
    "-framework MapKit"
    "-framework AppKit"
    "-framework CoreLocation"
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/bundle/Info.plist
    BUNDLE True
    MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME}
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.ubica.${CMAKE_PROJECT_NAME}"
)
