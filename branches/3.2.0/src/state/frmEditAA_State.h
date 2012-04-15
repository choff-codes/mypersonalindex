#ifndef FRMEDITAA_STATE_H
#define FRMEDITAA_STATE_H

#include "frmEditStateMap.h"

class QPoint;
class QModelIndex;
class frmEditAA_UI;
class assetAllocation;
class objectKeyEditModel;
class frmEditAA_State : public frmEditStateMap
{
    Q_OBJECT

public:
    frmEditAA_State(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent_ = 0);
    ~frmEditAA_State();

    void enter();
    void leave();
    bool validate();

private slots:
    void resetTarget();
    void customContextMenuRequested(const QPoint&);
    void listChange(const QModelIndex &current_, const QModelIndex&);
    void add();
    void remove();

private:
    frmEditAA_UI *ui;
    assetAllocation *m_currentItem;
    objectKeyEditModel *m_model;

    void load();
    void save();

    void validationError(const objectKeyBase &key_, const QString &errorMessage_);

    QString mimeType() const { return "application/mypersonalindex-assetallocation"; }
    bool internalCopy(QDataStream &stream_) const;
    void internalPaste(QDataStream &stream_);
};

#endif // FRMEDITAA_STATE_H
