project( lxqt-organizer )
cmake_minimum_required( VERSION 3.1 )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt5Widgets REQUIRED )
find_package ( Qt5Core REQUIRED )
find_package ( Qt5Gui REQUIRED )
find_package ( Qt5Sql REQUIRED )
set ( lxqt-organizer_HDRS
	dbcontroller.h
	simpledbusnotifier.h
	)

set ( lxqt-organizer_SRCS
	main.cpp
	mainwindow.cpp
	dialogsetappointment.cpp
	dbcontroller.cpp
	dialogabout.cpp
	simpledbusnotifier.cpp
	)

set ( lxqt-organizer_UIS
	mainwindow.ui
	dialogsetappointment.ui
	dialogabout.ui
	)
QT5_WRAP_UI(UIS ${lxqt-organizer_UIS})

set ( lxqt-organizer_MOCS
	mainwindow.h
	dialogsetappointment.h
	dialogabout.h
	)
QT5_WRAP_CPP(MOCS ${lxqt-organizer_MOCS})

add_executable ( lxqt-organizer ${lxqt-organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( lxqt-organizer  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql )

project (lxqt-organizer)
message ("CIP = ${CMAKE_INSTALL_PREFIX} defaults to /usr/local")
#set (CMAKE_INSTALL_PREFIX /opt/lxqt-organizer)
install (TARGETS lxqt-organizer DESTINATION ${CMAKE_INSTALL_PREFIX} )
