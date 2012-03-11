#ifndef FRMEDITSECURITY_STATE_H
#define FRMEDITSECURITY_STATE_H

#include "frmEditStateMap.h"

class QPoint;
class QModelIndex;
class frmEditSecurity_UI;
class security;
class objectKeyEditModel;
class frmEditSecurity_State : public frmEditStateMap
{
    Q_OBJECT

public:
    frmEditSecurity_State(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent_ = 0);
    ~frmEditSecurity_State();

    void enter();
    void leave();
    bool validate();

private slots:
    void resetExpenseRatio();
    void customContextMenuRequested(const QPoint&);
    void listChange(const QModelIndex &current_, const QModelIndex&);
    void add();
    void remove();
    void addAA();
    void deleteAA();

private:
    frmEditSecurity_UI *ui;
    security *m_currentItem;
    objectKeyEditModel *m_model;

    void load();
    void save();

    void validationError(const objectKeyBase &key_, const QString &errorMessage_);

    QString mimeType() const { return "application/mypersonalindex-security"; }
    bool internalCopy(QDataStream &stream_) const;
    void internalPaste(QDataStream &stream_);
};

#endif // FRMEDITSECURITY_STATE_H
