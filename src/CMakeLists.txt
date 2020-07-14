project( organizer )
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
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
set (organizer_HDRS
	appointment.h
	holiday.h
	daylistmodel.h
	dbmanager.h
	contact.h
	contactmodel.h
	schedulelistmodel.h
	birthdaylistmodel.h
	preferences.h	
	)

set ( organizer_SRCS
	
	main.cpp
	mainwindow.cpp	
	daylistmodel.cpp
	dbmanager.cpp
	dialogappointment.cpp
	contactmodel.cpp
	proxymodelcontacts.cpp
	dialogcontact.cpp	
	dialogupcomingschedule.cpp
	dialogupcomingbirthdays.cpp
	schedulelistmodel.cpp
	birthdaylistmodel.cpp
	dialogrepeatappointment.cpp
	dialogappointmentupdate.cpp
	dialogpreferences.cpp	
	)

set (organizer_UIS

mainwindow.ui
	dialogappointment.ui	
	dialogcontact.ui	
	dialogupcomingschedule.ui
	dialogupcomingbirthdays.ui
	dialogrepeatappointment.ui
	dialogappointmentupdate.ui
	dialogpreferences.ui
	)
QT5_WRAP_UI(UIS ${organizer_UIS})

set ( organizer_RSCS
	icons.qrc
	translations.qrc
	sounds.qrc
	)
QT5_ADD_RESOURCES(RSCS ${organizer_RSCS})

set ( organizer_MOCS
	
	mainwindow.h
	dialogappointment.h
	proxymodelcontacts.h	
	dialogcontact.h	
	dialogupcomingschedule.h	
	dialogupcomingbirthdays.h
	dialogrepeatappointment.h	
	dialogappointmentupdate.h
	dialogpreferences.h	
	)
QT5_WRAP_CPP(MOCS ${organizer_MOCS})

#-----------------------------------------------------------------------------
# Translations
#-----------------------------------------------------------------------------
lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES ${SOURCES} ${HEADERS} ${UIS}
    INSTALL_DIR "${ORG_TRANSLATIONS_DIR}"
)

qt5_translation_loader(
    QM_LOADER
    "${ORG_TRANSLATIONS_DIR}"
    ${PROJECT_NAME}
)

lxqt_translate_desktop(DESKTOP_FILES
    SOURCES "${CMAKE_SOURCE_DIR}/qps.desktop.in"
)
#-----------------------------------------------------------------------------
# End of translations
#-----------------------------------------------------------------------------

add_executable ( organizer ${organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} ${QM_FILES} ${QM_LOADER})
target_link_libraries ( organizer  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Svg Qt5::Xml Qt5::Multimedia Qt5::MultimediaWidgets)


INSTALL(TARGETS
    organizer
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)

