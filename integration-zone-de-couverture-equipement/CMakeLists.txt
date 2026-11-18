cmake_minimum_required(VERSION 3.16)
project(fire LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport Sql Charts Multimedia Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network PrintSupport Sql Charts Multimedia)
add_executable(fire
  main.cpp mainwindow.cpp mainwindow.h zone.cpp zone.h equipe.cpp equipe.h
  alerte.cpp alerte.h connection.cpp connection.h
  equipement.cpp equipement.h equipementswindow.cpp equipementswindow.h equipements.ui
  exports.cpp exports.h resources.qrc resources_equipment.qrc)
target_link_libraries(fire PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::PrintSupport Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Multimedia)
if(MSVC)
  target_compile_options(fire PRIVATE /utf-8)
endif()
