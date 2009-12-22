include(shared/qtsingleapplication/qtsingleapplication.pri)
include(forms/forms.pri)
include(classes/classes.pri)
include(models/models.pri)
include(ui/ui.pri)
include(usercontrols/usercontrols.pri)

INCLUDEPATH += shared/qwt

win32{
    CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwtd5.a
        else: LIBS += shared/qwt/lib/libqwt5.a
}

unix{
    CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwtd.so
        else: LIBS += shared/qwt/lib/libqwt.so
}

RESOURCES += icons/icons.qrc

QT += sql \
    network

SOURCES += main.cpp

unix {
    # in debug mode...
    CONFIG(debug, debug|release):DESTDIR = debug
    else:DESTDIR = release
}
