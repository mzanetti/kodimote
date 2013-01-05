TEMPLATE = subdirs


contains(MEEGO_EDITION,harmattan) {
    message(Harmattan build)
    SUBDIRS += harmattan
} else:maemo5{
    message(Fremantle build)
    SUBDIRS += generic
} else {
    message(Desktop build)
    SUBDIRS += generic
}
