#ifndef FRMEDITPORTFOLIO_STATE_H
#define FRMEDITPORTFOLIO_STATE_H

#include <QObject>
#include "frmEditState.h"

class frmEditPortfolio_UI;
class frmEditPortfolio_State : public QObject, public frmEditState
{
    Q_OBJECT
public:
    explicit frmEditPortfolio_State(portfolio portfolio_, QWidget *parent_ = 0);
    ~frmEditPortfolio_State();

    void enter() {} // no-op
    void leave() {} // no-op
    bool validate();

public slots:
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

#endif // FRMEDITPORTFOLIO_STATE_H
