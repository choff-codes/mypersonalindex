include(shared/qtsingleapplication/qtsingleapplication.pri)
include(forms/forms.pri)
include(classes/classes.pri)
include(models/models.pri)
include(ui/ui.pri)
include(usercontrols/usercontrols.pri)

INCLUDEPATH += shared/qwt

CONFIG(debug, debug|release): LIBS += shared/qwt/lib/libqwtd5.a
    else: LIBS += shared/qwt/lib/libqwt5.a

RESOURCES += icons/icons.qrc

QT += sql \
    network

SOURCES += main.cpp

unix {
    # in debug mode...
    CONFIG(debug, debug|release):DESTDIR = debug
    else:DESTDIR = release
}
