cmake_minimum_required(VERSION 3.16)

project(integration_empl_interv VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sql = base Oracle SAFESTATION (pilote QODBC)
# Multimedia = camera du Face ID (module Employes)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Multimedia)

set(PROJECT_SOURCES
    main.cpp
    appshell.cpp
    appshell.h
    connection.cpp
    connection.h
    # --- module Employes ---
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    employe.cpp
    employe.h
    # --- module Interventions ---
    gestioninterventions.cpp
    gestioninterventions.h
    gestioninterventions.ui
    intervention.cpp
    intervention.h
    affectation.cpp
    affectation.h
    graphiques.cpp
    graphiques.h
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(integration_empl_interv
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(integration_empl_interv ${PROJECT_SOURCES})
endif()

target_link_libraries(integration_empl_interv PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Multimedia)
target_include_directories(integration_empl_interv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(integration_empl_interv PRIVATE /utf-8)
endif()

set_target_properties(integration_empl_interv PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS integration_empl_interv
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(integration_empl_interv)
endif()
