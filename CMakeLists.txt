cmake_minimum_required(VERSION 3.16)
project(ReportsGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_BUNDLE "Build bundle" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(src/Core)
add_subdirectory(src/GUI)

set(SOURCES
    main.cpp
)

set(HEADERS)

set(RESOURCE
    qml.qrc
    resource.qrc
)

add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${RESOURCE}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    src
    src/Core
    src/GUI
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick

    "-framework Cocoa"
    "-framework MapKit"
    "-framework AppKit"
    "-framework CoreLocation"

    Core
    GUI
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    AUTORCC ON
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/bundle/Info.plist
)

if (BUILD_BUNDLE) 
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME}
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.ubica.${CMAKE_PROJECT_NAME}"
    )
endif()