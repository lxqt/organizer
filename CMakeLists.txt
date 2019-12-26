cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project( lxqt-organizer )

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Minimum Versions
set(LXQT_MINIMUM_VERSION "0.14.1")
set(QT_MINIMUM_VERSION "5.7.1")

find_package(Qt5Core ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5DBus ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Gui ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Widgets ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Sql ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Svg ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Xml ${QT_MINIMUM_VERSION} REQUIRED)
find_package(lxqt ${LXQT_MINIMUM_VERSION} REQUIRED)
message(STATUS "Building with Qt${Qt5Core_VERSION}")

# Version
set(LXQT_ORGANIZER_MAJOR_VERSION 0)
set(LXQT_ORGANIZER_MINOR_VERSION 0)
set(LXQT_ORGANIZER_PATCH_VERSION 1)

set(LXQT_ORGANIZER_VERSION ${LXQT_ORGANIZER_MAJOR_VERSION}.${LXQT_ORGANIZER_MINOR_VERSION}.${LXQT_ORGANIZER_PATCH_VERSION})
add_definitions("-DLXQT_ORGANIZER_VERSION=\"${LXQT_ORGANIZER_VERSION}\"")

# include(LXQtPreventInSourceBuilds)

# leave our compiler settings out for now, some fixes needed
# include(LXQtCompilerSettings NO_POLICY_SCOPE)

# Translations **********************************
# include(LXQtTranslate)

add_subdirectory(src)
add_subdirectory(scripts)

# install(FILES
#     man/lxqt-organzier.1
#     DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
#     COMPONENT Runtime
# )

