project(lxqt-organizer)

set (lxqt-organizer_HDRS
     dbcontroller.h
     simpledbusnotifier.h
)

set(lxqt-organizer_SRCS
    main.cpp
    mainwindow.cpp
    dialogsetappointment.cpp
    dbcontroller.cpp
    dialogabout.cpp
    simpledbusnotifier.cpp
)

set(lxqt-organizer_UIS
    mainwindow.ui
    dialogsetappointment.ui
    dialogabout.ui
)
QT5_WRAP_UI(UIS ${lxqt-organizer_UIS})

set(lxqt-organizer_MOCS
    mainwindow.h
    dialogsetappointment.h
    dialogabout.h
)
QT5_WRAP_CPP(MOCS ${lxqt-organizer_MOCS})

add_executable(lxqt-organizer ${lxqt-organizer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS})
target_link_libraries(lxqt-organizer  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql)
