include(shared/qtsingleapplication.pri)
RESOURCES += icons/icons.qrc
QT += sql \
    network
SOURCES += main.cpp \
    ui/frmMain_UI.cpp \
    forms/frmMain.cpp \
    classes/queries.cpp \
    classes/globals.cpp \
    classes/updatePrices.cpp \
    forms/frmPortfolio.cpp \
    ui/frmPortfolio_UI.cpp \
    classes/queries/mainQueries.cpp \
    classes/queries/portfolioQueries.cpp \
    ui/frmTicker_UI.cpp \
    forms/frmTicker.cpp
HEADERS += frmMain_UI.h \
    ui/frmMain_UI.h \
    forms/frmMain.h \
    classes/queries.h \
    classes/globals.h \
    classes/updatePrices.h \
    forms/frmPortfolio.h \
    ui/frmPortfolio_UI.h \
    classes/queries/mainQueries.h \
    classes/queries/portfolioQueries.h \
    classes/functions.h \
    usercontrols/mpiDoubleValidator.h \
    ui/frmTicker_UI.h \
    forms/frmTicker.h \
    usercontrols/dateEditDelegate.h
INCLUDEPATH += forms \
    usercontrols \
    ui \
    classes \
    classes/queries \
    shared
RC_FILE = MyPersonalIndex_resource.rc
