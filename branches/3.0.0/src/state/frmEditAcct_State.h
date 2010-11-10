#ifndef FRMEDITACCT_STATE_H
#define FRMEDITACCT_STATE_H

#include <QObject>
#include "frmEditStateMap.h"

class QPoint;
class QModelIndex;
class frmEditAcct_UI;
class account;
class objectKeyEditModel;
class frmEditAcct_State : public QObject, public frmEditStateMap
{
    Q_OBJECT
public:
    explicit frmEditAcct_State(portfolio portfolio_, QWidget *parent_ = 0);
    ~frmEditAcct_State();

    void enter();
    void leave();
    bool validate();

public slots:
    void save();

private slots:
    void resetTaxRate();
    void customContextMenuRequested(const QPoint&);
    void listChange(const QModelIndex &current_, const QModelIndex &previous_);
    void copySlot() { copy(); }
    void pasteSlot() { paste(); }
    void add();
    void remove();

private:
    frmEditAcct_UI *ui;
    account *m_currentItem;
    objectKeyEditModel *m_model;

    void load();

    void validationError(objectKey* key_, const QString &errorMessage_);

    QString mimeType() { return "application/mypersonalindex-account"; }
    bool internalCopy(QDataStream &stream_);
    void internalPaste(QDataStream &stream_);
};


#endif // FRMEDITACCT_STATE_H
