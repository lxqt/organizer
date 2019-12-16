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
find_package ( Qt5DBus REQUIRED )
find_package(Qt5Svg REQUIRED)
find_package(Qt5Xml REQUIRED)

set ( lxqt-organizer_HDRS
	appointment.h
	contact.h
	dbmanager.h
	reminder.h
	contactmodel.h
	holiday.h
	reminderslistmodel.h
	daylistmodel.h
	)

set ( lxqt-organizer_SRCS
	main.cpp
	mainwindow.cpp
	dbmanager.cpp
	dialogappointment.cpp
	dialogshowreminders.cpp
	dbussessionmessage.cpp
	contactmodel.cpp
	proxymodelcontacts.cpp
	dialogcontact.cpp
	reminderslistmodel.cpp
	daylistmodel.cpp
	dialogabout.cpp
	)

set ( lxqt-organizer_UIS
	mainwindow.ui
	dialogappointment.ui
	dialogshowreminders.ui
	dialogcontact.ui
	dialogabout.ui
	)
QT5_WRAP_UI(UIS ${lxqt-organizer_UIS})

set ( lxqt-organizer_RSCS
	icons.qrc
	)
QT5_ADD_RESOURCES(RSCS ${lxqt-organizer_RSCS})

set ( lxqt-organizer_MOCS
	mainwindow.h
	dialogappointment.h
	dialogshowreminders.h
	dbussessionmessage.h
	proxymodelcontacts.h
	dialogcontact.h
	dialogabout.h
	)
QT5_WRAP_CPP(MOCS ${lxqt-organizer_MOCS})

add_executable ( lxqt-organizer ${lxqt-organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( lxqt-organizer  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::DBus Qt5::Svg Qt5::Xml)

install(TARGETS lxqt-organizer DESTINATION ${CMAKE_INSTALL_PREFIX})
