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
	appointmentmodel.h
	contact.h
	contactmodel.h
	dbmanager.h
	remindermodel.h
	)

set ( lxqt-organizer_SRCS
	main.cpp
	mainwindow.cpp
	appointmentmodel.cpp
	contactmodel.cpp
	dbmanager.cpp
	dialogaddappointment.cpp
	dialogaddcontact.cpp
	proxymodel.cpp
	proxymodelcontacts.cpp
	remindermodel.cpp
	dbussessionmessage.cpp
	dialogabout.cpp
	)

set ( lxqt-organizer_UIS
	mainwindow.ui
	dialogaddappointment.ui
	dialogaddcontact.ui
	dialogabout.ui
	)
QT5_WRAP_UI(UIS ${lxqt-organizer_UIS})

set ( lxqt-organizer_RSCS
	icons.qrc
	)
QT5_ADD_RESOURCES(RSCS ${lxqt-organizer_RSCS})

set ( lxqt-organizer_MOCS
	mainwindow.h
	dialogaddappointment.h
	dialogaddcontact.h
	proxymodel.h
	proxymodelcontacts.h
	dbussessionmessage.h
	dialogabout.h
	)
QT5_WRAP_CPP(MOCS ${lxqt-organizer_MOCS})

INCLUDE_DIRECTORIES( ${Qt5Xml_INCLUDE_DIRS} )

add_executable ( lxqt-organizer ${lxqt-organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( lxqt-organizer  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::DBus Qt5::Svg Qt5::Xml )
