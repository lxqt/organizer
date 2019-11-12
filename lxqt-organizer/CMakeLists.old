project(lxqt-organizer)

set (lxqt-organizer_HDRS
    appointment.h
    appointmentmodel.h
    dbmanager.h
    dbussessionmessage.h
    dialogabout.h
    dialogaddappointment.h
    mainwindow.h
    proxymodel.h
   remindermodel.h
)

set(lxqt-organizer_SRCS
    appointmentmodel.cpp
    dbmanager.cpp
    dbussessionmessage.cpp
    dialogabout.cpp
    dialogaddappointment.cpp
    main.cpp
    mainwindow.cpp
    proxymodel.cpp
    remindermodel.cpp
)

set(lxqt-organizer_UIS
   dialogabout.ui
   dialogaddappointment.ui
   mainwindow.ui
)
QT5_WRAP_UI(UIS ${lxqt-organizer_UIS})

set(lxqt-organizer_MOCS
    dialogabout.h
    dialogaddappointment.h
    mainwindow.h
)
QT5_WRAP_CPP(MOCS ${lxqt-organizer_MOCS})

add_executable(lxqt-organizer ${lxqt-organizer_HDRS} ${lxqt-organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS})
target_link_libraries(lxqt-organizer  Qt5::Core Qt5::DBus Qt5::Gui Qt5::Widgets Qt5::Sql)
