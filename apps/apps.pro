TEMPLATE = subdirs


contains(MEEGO_EDITION,harmattan) {
    message("Harmattan build. Building harmattan app.")
    SUBDIRS += harmattan
}
maemo5{
    message("Fremantle build. Building generic app.")
    SUBDIRS += generic
}

ubuntu {
    message("Desktop Qt5 build. Building Ubuntu version.")
    SUBDIRS += ubuntu
}

exists("/usr/include/sailfishapp/sailfishapp.h") {
    message("Sailfish build. Building sailfish app.")
    SUBDIRS += sailfish
}

simulator {
    message("Simulator build. Building harmattan app.")
    SUBDIRS += harmattan
}

contains (QT_VERSION, ^4\\..\\..*) {
	message("Desktop Qt4 build. Building generic app.")
	SUBDIRS += generic
}
