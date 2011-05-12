#ifndef FRMEDITPORTFOLIO_STATE_H
#define FRMEDITPORTFOLIO_STATE_H

#include "frmEditState.h"

class frmEditPortfolio_UI;
class frmEditPortfolio_State : public frmEditState
{
public:
    frmEditPortfolio_State(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent_ = 0);
    ~frmEditPortfolio_State();

    void enter() {} // no-op
    void leave() { save(); }
    bool validate();

private:
    frmEditPortfolio_UI *ui;

    QString mimeType() const { return QString(); } // no-op
    bool internalCopy(QDataStream&) const { return false; } // no-op
    void internalPaste(QDataStream&) {} // no-op

    void add() {} // no-op
    void load();
    void save();
    void remove() {} // no-op
};

#endif // FRMEDITPORTFOLIO_STATE_H
