TEMPLATE = subdirs


contains(MEEGO_EDITION,harmattan) {
    message("Harmattan build. Building harmattan app.")
    SUBDIRS += harmattan
} else:maemo5{
    message("Fremantle build. Building generic app.")
    SUBDIRS += generic
} else:contains(QT_VERSION, ^5\\..\\..*) {
    message("Desktop Qt5 build. Building Ubuntu version.")
    SUBDIRS += ubuntu
} else:simulator {
    message("Simulator build. Building harmattan app.")
    SUBDIRS += harmattan
} else {
    message("Desktop Qt4 build. Building generic app.")
    SUBDIRS += generic
}
