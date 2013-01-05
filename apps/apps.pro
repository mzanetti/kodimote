TEMPLATE = subdirs


contains(MEEGO_EDITION,harmattan) {
    message(Harmattan build)
    SUBDIRS += harmattan
} else:maemo5{
    message(Fremantle build)
    SUBDIRS += generic
} else:contains(QT_VERSION, ^5\\..\\..*) {
    message("Qt5 build. Using Ubuntu compoenents for now")
    SUBDIRS += ubuntu
} else {
    message(Desktop build)
    SUBDIRS += generic
}
