#ifndef FRMMAINSTATE_H
#define FRMMAINSTATE_H

#include <QObject>
#include "portfolio.h"
#include "calculatorNAV.h"

class frmMainState : public QObject
{
public:
    frmMainState(const portfolio &portfolio_, const calculatorNAV &calculator_, QObject *parent_):
        QObject(parent_),
        m_portfolio(portfolio_),
        m_calculator(calculator_)
    {}

    virtual ~frmMainState() {}

    virtual QWidget* mainWidget() = 0;

protected:
    portfolio m_portfolio;
    calculatorNAV m_calculator;
};

#endif // FRMMAINSTATE_H
