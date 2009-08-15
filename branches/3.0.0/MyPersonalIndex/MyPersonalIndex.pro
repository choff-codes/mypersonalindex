RESOURCES += icons/icons.qrc
QT += sql network
SOURCES += main.cpp \
    ui/frmMain_UI.cpp \
    forms/frmMain.cpp \
    classes/queries.cpp \
    classes/globals.cpp \
    classes/updatePrices.cpp
HEADERS += frmMain_UI.h \
    usercontrols/mpiToolButton.h \
    ui/frmMain_UI.h \
    forms/frmMain.h \
    classes/queries.h \
    classes/globals.h \
    classes/updatePrices.h
INCLUDEPATH += forms \
    usercontrols \
    ui \
    classes
