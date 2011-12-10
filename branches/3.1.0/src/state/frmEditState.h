#ifndef FRMEDITSTATE_H
#define FRMEDITSTATE_H

#include <QObject>
#include "portfolio.h"
#include "fileStateIdentity.h"

class QDataStream;
class frmEditState : public QObject
{
    Q_OBJECT

public:
    frmEditState(const portfolio &portfolio_, const fileStateIdentity &identities_, QObject *parent_):
        QObject(parent_),
        m_portfolio(portfolio_),
        m_identities(identities_)
    {}

    virtual ~frmEditState() {}

    virtual bool validate() = 0;
    virtual void enter() = 0;
    virtual void leave() = 0;

public slots:
    virtual void save() = 0;

protected slots:
    void copy();
    void paste();

protected:
    portfolio m_portfolio;
    fileStateIdentity m_identities;
    static const int m_magicNumber;

    virtual QString mimeType() const = 0;
    virtual bool internalCopy(QDataStream &stream_) const = 0;
    virtual void internalPaste(QDataStream &stream_) = 0;

    virtual void add() = 0;
    virtual void load() = 0;
    virtual void remove() = 0;
};

#endif // FRMEDITSTATE_H
