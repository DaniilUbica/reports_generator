cmake_minimum_required(VERSION 3.16)
set(UNIT_NAME ReportsGeneratorTests)

add_subdirectory(googletest)

set(SOURCES
    test_main.cpp
)

add_executable(${UNIT_NAME}
    ${SOURCES}
)

target_include_directories(${UNIT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(${UNIT_NAME} PRIVATE
    ${CMAKE_PROJECT_NAME}
    gtest_main
    gmock
)

include(GoogleTest)
gtest_discover_tests(${UNIT_NAME})

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(${UNIT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework MapKit"
        "-framework CoreLocation"
    )
endif()