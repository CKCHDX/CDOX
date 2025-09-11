cmake_minimum_required(VERSION 3.15)
project(cdox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(cdox
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
)
add_custom_target(run
    COMMAND cdox
    DEPENDS cdox
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(cdox PRIVATE Qt6::Widgets)
