include(shared/qtsingleapplication/qtsingleapplication.pri)
include(forms/forms.pri)
include(classes/classes.pri)
include(models/models.pri)
include(ui/ui.pri)
include(usercontrols/usercontrols.pri)

INCLUDEPATH += shared/qwt

win32{
    CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwtd6.a
        else: LIBS += shared/qwt/lib/libqwt6.a
}

unix{
    CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwtd.so
        else: LIBS += shared/qwt/lib/libqwt.so
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
