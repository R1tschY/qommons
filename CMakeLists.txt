cmake_minimum_required(VERSION 3.5)
project(qommons VERSION 0.1 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(QOMMONS_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
#set(QML_IMPORT_PATH "${QML_IMPORT_PATH}" PARENT_SCOPE)

include(GNUInstallDirs)

set(QOMMONS_QML_INSTALL_PATH "${CMAKE_INSTALL_DATADIR}/qml" CACHE PATH "Path where to install QML files")

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Qml Quick QuickTest REQUIRED)

# Http
add_custom_target(qommons-http
    SOURCES
        Qommons/Http/qmldir
        Qommons/Http/HttpRequest.qml)
install(
    DIRECTORY Qommons/Http/
    DESTINATION share/harbour-command/qml
)

# Lang
add_custom_target(qommons-lang
    SOURCES
        Qommons/Lang/qmldir
        Qommons/Lang/Object.qml
        Qommons/Lang/StringUtils.js)
install(
    DIRECTORY Qommons/Lang/
    DESTINATION share/harbour-command/qml
)

# Database
add_custom_target(qommons-database
    SOURCES
        Qommons/Database/qmldir
        Qommons/Database/Repository.qml)
install(
    DIRECTORY Qommons/Database/
    DESTINATION share/harbour-command/qml
)

# Process
add_library(
    qommons-process STATIC
    Qommons/Process/Types.h
    Qommons/Process/Types.cpp
    Qommons/Process/Process.h
    Qommons/Process/Process.cpp
)
target_link_libraries(qommons-process Qt5::Core Qt5::Qml)
target_include_directories(qommons-process PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Models
add_subdirectory(Qommons/Models)

# UUID
add_library(
    qommons-uuid STATIC
    Qommons/Uuid/Types.h
    Qommons/Uuid/Types.cpp
    Qommons/Uuid/Uuid.h
    Qommons/Uuid/Uuid.cpp
)
target_link_libraries(qommons-uuid Qt5::Core Qt5::Qml)
target_include_directories(qommons-uuid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
