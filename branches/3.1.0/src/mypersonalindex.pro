include(forms/forms.pri)
include(classes/classes.pri)
include(state/state.pri)
include(models/models.pri)
include(ui/ui.pri)
include(usercontrols/usercontrols.pri)
include(prices/prices.pri)
include(calculators/calculators.pri)

VERSION = 310
DEFINES += APP_VERSION="$$VERSION"

win32{
    INCLUDEPATH += shared/qwt

    CONFIG(debug, debug|release): LIBS += ../src/shared/qwt/lib/libqwtd5.a
        else: LIBS += ../src/shared/qwt/lib/libqwt5.a
}

unix:!macx{
    INCLUDEPATH += /usr/include/qwt/
    LIBS += /usr/lib/libqwt.so
}

macx{
    INCLUDEPATH += shared/qwt
    LIBS += shared/qwt/lib/libqwt.dylib
}

CONFIG(debug, debug|release): DEFINES += CLOCKTIME

RESOURCES += icons/icons.qrc

QT += sql \
    network

SOURCES += main.cpp
RC_FILE = mypersonalindex.rc

unix {
    # in debug mode...
    CONFIG(debug, debug|release):DESTDIR = debug
    else:DESTDIR = release
}
