#ifndef FRMTRADE_H
#define FRMTRADE_H

#include <QtGui>
#include "frmTrade_UI.h"
#include "globals.h"

class frmTrade : public QDialog
{
    Q_OBJECT

public:

    const globals::dynamicTrade& getReturnValues() const { return m_trade; }

    frmTrade(QWidget *parent = 0, const QMap<int, globals::security> *securities = 0, const int &tickerID = -1, const globals::dynamicTrade &trade = globals::dynamicTrade());

private:

    frmTrade_UI ui;
    globals::dynamicTrade m_trade;
    QString m_oldPrice; // store txtPrice to restore if chkPrice is checked

    void connectSlots();
    void loadTrade();

private slots:
    void togglePrice(bool);
    void accept();
    void freqChange(int);
    void typeChange(int);
};

#endif // FRMTRADE_H
