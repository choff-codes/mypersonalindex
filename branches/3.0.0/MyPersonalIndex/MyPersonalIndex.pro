RESOURCES += icons/icons.qrc
QT += sql
SOURCES += main.cpp \
    ui/frmMain_UI.cpp \
    forms/frmMain.cpp
HEADERS += frmMain_UI.h \
    usercontrols/mpiToolButton.h \
    ui/frmMain_UI.h \
    forms/frmMain.h
INCLUDEPATH += forms usercontrols ui
