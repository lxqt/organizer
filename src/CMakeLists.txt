project( Organizer )
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
find_package(Qt5Svg REQUIRED)
find_package(Qt5Xml REQUIRED)
set ( Organizer_HDRS
	appointment.h
	daylistmodel.h
	dbmanager.h
	contact.h
	holiday.h
	birthdaylistmodel.h
	contactmodel.h
	schedulelistmodel.h
	)

set ( Organizer_SRCS
	main.cpp
	mainwindow.cpp
	daylistmodel.cpp
	dialogappointment.cpp
	dbmanager.cpp
	birthdaylistmodel.cpp
	dialogabout.cpp
	dialogcontact.cpp
	dialogrepeatappointment.cpp
	dialogupcomingbirthdays.cpp
	dialogupcomingschedule.cpp
	contactmodel.cpp
	proxymodelcontacts.cpp
	schedulelistmodel.cpp
	dialogappointmentupdate.cpp
	)

set ( Organizer_UIS
	mainwindow.ui
	dialogappointment.ui
	dialogabout.ui
	dialogcontact.ui
	dialogrepeatappointment.ui
	dialogupcomingbirthdays.ui
	dialogupcomingschedule.ui
	dialogappointmentupdate.ui
	)
QT5_WRAP_UI(UIS ${Organizer_UIS})

set ( Organizer_RSCS
	icons.qrc
	)
QT5_ADD_RESOURCES(RSCS ${Organizer_RSCS})

set ( Organizer_MOCS
	mainwindow.h
	dialogappointment.h
	dialogabout.h
	dialogcontact.h
	dialogrepeatappointment.h
	dialogupcomingbirthdays.h
	dialogupcomingschedule.h
	proxymodelcontacts.h
	dialogappointmentupdate.h
	)
QT5_WRAP_CPP(MOCS ${Organizer_MOCS})

add_executable ( organizer ${organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( organizer  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Svg Qt5::Xml )

#Testing purposes..
#install(TARGETS organizer DESTINATION ${CMAKE_INSTALL_PREFIX})

#INSTALL(TARGETS
    #organizer
   #RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
   #COMPONENT Runtime
#)

