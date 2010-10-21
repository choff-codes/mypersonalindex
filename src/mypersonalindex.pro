include(forms/forms.pri)
include(classes/classes.pri)
include(models/models.pri)
include(ui/ui.pri)
include(usercontrols/usercontrols.pri)

VERSION = 310
VERSIONTEXT = "3.1.0"
DEFINES += APP_VERSION="$$VERSION" \
        APP_VERSION_TEXT="$$VERSIONTEXT"


win32{
    INCLUDEPATH += shared/qwt

    CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwtd6.a
        else: LIBS += shared/qwt/lib/libqwt6.a
}

unix:!macx{
    INCLUDEPATH += /usr/include/qwt/
    LIBS += /usr/lib/libqwt.so
}

macx{
    INCLUDEPATH += shared/qwt

    CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwt.dylib
        else: LIBS += shared/qwt/lib/libqwt.dylib
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
