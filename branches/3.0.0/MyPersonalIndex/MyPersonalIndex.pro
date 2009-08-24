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
    classes/queries/portfolioQueries.cpp
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
    classes/functions.h
INCLUDEPATH += forms \
    usercontrols \
    ui \
    classes \
    classes/queries
RC_FILE = MyPersonalIndex_resource.rc
