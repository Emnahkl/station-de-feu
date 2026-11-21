cmake_minimum_required(VERSION 3.16)
project(campagnes_sensibilisation VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Charts)

qt_add_executable(campagnes_sensibilisation
    main.cpp
    campagne.cpp campagne.h
    campagneswindow.cpp campagneswindow.h
    campagnemetiers.cpp campagnemetiers.h
    gifwriter.cpp gifwriter.h
    connection.cpp connection.h
    menulateral.cpp menulateral.h
    resources.qrc
)
target_link_libraries(campagnes_sensibilisation PRIVATE Qt6::Widgets Qt6::Sql Qt6::Charts)
set_target_properties(campagnes_sensibilisation PROPERTIES WIN32_EXECUTABLE ON)
