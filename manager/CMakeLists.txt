cmake_minimum_required(VERSION 3.5)

project(fractal_manager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -pthread")

include_directories(inc/)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(fractal_manager
  src/mainwindow.ui
  src/main.cpp
  src/mainwindow.cpp
  src/Logger.cpp
  src/Socket.cpp
  src/Client.cpp
  src/Server.cpp
  inc/mainwindow.h
  inc/Logger.h
  inc/Socket.h
  inc/Client.h
  inc/Server.h
)

target_link_libraries(fractal_manager PRIVATE Qt5::Widgets)
