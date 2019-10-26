project(desktop)

file(
    GLOB DESKTOP_FILES *.desktop
)

install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    COMPONENT Runtime
)
