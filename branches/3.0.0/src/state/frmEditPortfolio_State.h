#ifndef FRMEDITSTATE_PORTFOLIO_H
#define FRMEDITSTATE_PORTFOLIO_H

#include "frmEditState.h"

class QWidget;
class frmEditPortfolio_UI;
class frmEditPortfolio_State : public frmEditState
{
public:
    explicit frmEditPortfolio_State(portfolio portfolio_, QWidget *parent_ = 0);
    ~frmEditPortfolio_State();

    void enter() {} // no-op
    void leave() {} // no-op
    bool validate();
    void save();

private:
    frmEditPortfolio_UI *ui;

    QString mimeType() { return QString(); } // no-op
    bool internalCopy(QDataStream&) { return false; } // no-op
    void internalPaste(QDataStream&) {} // no-op

    void add() {} // no-op
    void load();
    void remove() {} // no-op
};

#endif // FRMEDITSTATE_PORTFOLIO_H
