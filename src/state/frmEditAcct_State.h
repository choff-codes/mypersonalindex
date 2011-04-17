#ifndef FRMEDITACCT_STATE_H
#define FRMEDITACCT_STATE_H

#include "frmEditStateMap.h"

class QPoint;
class QModelIndex;
class frmEditAcct_UI;
class account;
class objectKeyEditModel;
class frmEditAcct_State : public frmEditStateMap
{
    Q_OBJECT
public:
    frmEditAcct_State(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent_ = 0);
    ~frmEditAcct_State();

    void enter();
    void leave();
    bool validate();

private slots:
    void resetTaxRate();
    void customContextMenuRequested(const QPoint&);
    void listChange(const QModelIndex &current_, const QModelIndex&);
    void add();
    void remove();

private:
    frmEditAcct_UI *ui;
    account *m_currentItem;
    objectKeyEditModel *m_model;

    void load();
    void save();

    void validationError(const objectKeyBase &key_, const QString &errorMessage_);

    QString mimeType() const { return "application/mypersonalindex-account"; }
    bool internalCopy(QDataStream &stream_) const;
    void internalPaste(QDataStream &stream_);
};


#endif // FRMEDITACCT_STATE_H
