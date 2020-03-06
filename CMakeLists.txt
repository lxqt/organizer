cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project( organizer )

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
set(ORGANIZER_MAJOR_VERSION 0)
set(ORGANIZER_MINOR_VERSION 0)
set(ORGANIZER_PATCH_VERSION 1)

set(ORGANIZER_VERSION ${ORGANIZER_MAJOR_VERSION}.${ORGANIZER_MINOR_VERSION}.${ORGANIZER_PATCH_VERSION})
add_definitions("-DORGANIZER_VERSION=\"${ORGANIZER_VERSION}\"")

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)

# Added by LXQtCompilerSettings. Code refactoring need before using them
remove_definitions(
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_FOREACH
)

# Translations **********************************
# include(LXQtTranslate)

add_subdirectory(src)
add_subdirectory(scripts)

# install(FILES
#     man/organzier.1
#     DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
#    COMPONENT Runtime
# )

