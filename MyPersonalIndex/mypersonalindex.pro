include(shared/qtsingleapplication/qtsingleapplication.pri)
include(forms/forms.pri)
include(classes/classes.pri)
include(classes/queries/queries.pri)
include(ui/ui.pri)
include(usercontrols/usercontrols.pri)
RESOURCES += icons/icons.qrc
QT += sql \
    network
SOURCES += main.cpp
RC_FILE = MyPersonalIndex_resource.rc
unix { 
    # in debug mode...
    CONFIG(debug, debug|release):DESTDIR = debug
    else:DESTDIR = release
}
