#ifndef FRMTRADE_H
#define FRMTRADE_H

#include <QtGui>
#include "frmTrade_UI.h"
#include "security.h"

class frmTrade : public QDialog
{
    Q_OBJECT

public:

    const trade& getReturnValues() const { return m_trade; }

    frmTrade(const QMap<int, security> &cashAccounts, QWidget *parent = 0, const trade &trade = trade());

private:

    frmTrade_UI ui;
    trade m_trade;
    QString m_oldPrice; // store txtPrice to restore if chkPrice is checked

    void connectSlots();
    void loadTrade();
    bool hasValidationErrors();

private slots:
    void togglePrice(bool);
    void accept();
    void freqChange(int);
    void typeChange(int);
};

#endif // FRMTRADE_H
