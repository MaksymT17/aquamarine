cmake_minimum_required(VERSION 3.16)
project(AquamarineQt VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
find_package(spdlog REQUIRED)

# Need to include the aquamarine headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../silber/include
)

add_executable(AquamarineQt
    main.cpp
    mainwindow.cpp
    worker.cpp
    assets.qrc
)

target_link_libraries(AquamarineQt PRIVATE 
    Qt6::Widgets
    Qt6::Svg
    aquamarine_lib
    spdlog::spdlog
    silber-1.2
    ${CMAKE_THREAD_LIBS_INIT}
)
IF (UNIX OR APPLE)
target_link_libraries(AquamarineQt PRIVATE ${SQLite3_LIBRARIES} ${JPEG_LIBRARIES})
ENDIF()

if(WIN32)
    set_target_properties(AquamarineQt PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
