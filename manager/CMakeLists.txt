cmake_minimum_required(VERSION 3.5)

project(fractal_manager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -pthread")

set(SRC_COM_DIR "../common/src/")
set(SRC_COM ${SRC_COM_DIR}Logger.cpp)

set(INC_COM_DIR "../common/inc/")
set(INC_COM ${INC_COM_DIR}Logger.h)

add_library(common STATIC ${SRC_COM} ${INC_COM})
target_include_directories(common PRIVATE ${INC_COM_DIR})

set(SRC_COM_DIR "../common/src/Net/")
set(SRC_COM_NET
  ${SRC_COM_DIR}Socket.cpp
  ${SRC_COM_DIR}Client.cpp
  ${SRC_COM_DIR}Server.cpp
)

set(INC_COM_NET_DIR "../common/inc/Net/")
set(INC_COM_NET
  ${INC_COM_NET_DIR}Socket.h
  ${INC_COM_NET_DIR}Client.h
  ${INC_COM_NET_DIR}Server.h
)

add_library(common_net STATIC ${SRC_COM_NET} ${INC_COM_NET})
target_include_directories(common_net PRIVATE ${INC_COM_DIR} ${INC_COM_NET_DIR})
target_link_libraries(common_net)

set(SRC_DIR "src/")
set(SRC
  ${SRC_DIR}mainwindow.ui
  ${SRC_DIR}main.cpp
  ${SRC_DIR}mainwindow.cpp
)
set(INC_DIR "inc/")
set(INC ${INC_DIR}mainwindow.h)

add_executable(fractal_manager ${SRC} ${INC})
target_include_directories(fractal_manager PRIVATE ${INC_DIR} ${INC_COM_DIR})

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(fractal_manager common common_net Qt5::Widgets)
