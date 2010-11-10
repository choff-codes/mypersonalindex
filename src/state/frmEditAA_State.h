#ifndef FRMEDITAA_STATE_H
#define FRMEDITAA_STATE_H

#include <QObject>
#include "frmEditStateMap.h"

class QPoint;
class QModelIndex;
class frmEditAA_UI;
class assetAllocation;
class objectKeyEditModel;
class frmEditAA_State : public QObject, public frmEditStateMap
{
    Q_OBJECT
public:
    explicit frmEditAA_State(portfolio portfolio_, QWidget *parent_ = 0);
    ~frmEditAA_State();

    void enter();
    void leave();
    bool validate();

public slots:
    void save();

private slots:
    void resetTarget();
    void customContextMenuRequested(const QPoint&);
    void listChange(const QModelIndex &current_, const QModelIndex &previous_);
    void copySlot() { copy(); }
    void pasteSlot() { paste(); }
    void add();
    void remove();

private:
    frmEditAA_UI *ui;
    assetAllocation *m_currentItem;
    objectKeyEditModel *m_model;

    void validationError(objectKey* key_, const QString &errorMessage_);

    QString mimeType() { return "application/mypersonalindex-assetallocation"; }
    bool internalCopy(QDataStream &stream_);
    void internalPaste(QDataStream &stream_);

    void load();
};

#endif // FRMEDITAA_STATE_H
