cmake_minimum_required(VERSION 3.14)

project(KilobotOhcLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(examples)

include_directories( include)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialPort)

add_library(KilobotOhcLib SHARED
        include/KilobotOhcLib_global.h
        src/kilobotohclib.cpp
        include/kilobotohclib.h
        include/packet.h
        src/kilobotoverheadcontroller.cpp include/kilobotoverheadcontroller.h src/serialconnection.cpp include/serialconnection.h)

target_link_libraries(KilobotOhcLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::SerialPort )

target_compile_definitions(KilobotOhcLib PRIVATE KILOBOTOHCLIB_LIBRARY)
