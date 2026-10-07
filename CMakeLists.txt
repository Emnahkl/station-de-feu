cmake_minimum_required(VERSION 3.16)
project(FireStationZones VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Fonctionne avec Qt6 ou Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.ui)
set(RESOURCES resources/resources.qrc)

add_executable(FireStationZones ${SOURCES} ${RESOURCES})
target_include_directories(FireStationZones PRIVATE src)
target_link_libraries(FireStationZones PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
