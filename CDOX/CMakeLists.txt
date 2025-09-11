cmake_minimum_required(VERSION 3.15)
project(cdox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network)

add_executable(cdox
    src/main.cpp
    src/MainWindow.cpp
    src/backend/SystemLogWatcher.cpp
    src/NetworkTopologyWidget.cpp
    src/NetworkScanner.cpp
    src/NetworkNodeItem.cpp
    src/MainWindow.h
    src/backend/SystemLogWatcher.h
    src/NetworkTopologyWidget.h
    src/NetworkScanner.h
    src/NetworkNodeItem.h
)

add_custom_target(run
    COMMAND cdox
    DEPENDS cdox
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(cdox PRIVATE Qt6::Widgets Qt6::Network Qt6::Core)
