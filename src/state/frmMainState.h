#ifndef FRMMAINSTATE_H
#define FRMMAINSTATE_H

#include <QObject>
#include "portfolio.h"

class frmMainState : public QObject
{
public:
    frmMainState(const portfolio &portfolio_, QObject *parent_):
        QObject(parent_),
        m_portfolio(portfolio_)
    {}

    virtual ~frmMainState() {}

    virtual QWidget* mainWidget() = 0;

protected:
    portfolio m_portfolio;
};

#endif // FRMMAINSTATE_H
