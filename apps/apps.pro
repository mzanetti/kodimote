TEMPLATE = subdirs


contains(MEEGO_EDITION,harmattan) {
    message(Harmattan build)
    SUBDIRS += harmattan
} else {
    message(Desktop build)
    SUBDIRS += generic
}
