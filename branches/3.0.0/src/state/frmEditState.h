#ifndef FRMEDITSTATE_H
#define FRMEDITSTATE_H

#include "portfolio.h"

class QDataStream;
class objectKey;
class frmEditState
{
public:
    frmEditState(portfolio portfolio_):
        m_portfolio(portfolio_)
    {}

    virtual ~frmEditState() {}

    virtual void enter() = 0;
    virtual void leave() = 0;
    virtual void save() = 0;
    virtual bool validate() = 0;

protected:
    portfolio m_portfolio;
    static const int m_magicNumber;

    void copy();
    void paste();

    virtual QString mimeType() = 0;
    virtual bool internalCopy(QDataStream &stream_) = 0;
    virtual void internalPaste(QDataStream &stream_) = 0;

    virtual void add() = 0;
    virtual void load() = 0;
    virtual void remove() = 0;
};

#endif // FRMEDITSTATE_H
