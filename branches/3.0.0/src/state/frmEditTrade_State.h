#ifndef FRMEDITTRADE_STATE_H
#define FRMEDITTRADE_STATE_H

#include "frmEditStateMap.h"

class QPoint;
class QModelIndex;
class frmEditTrade_UI;
class trade;
class objectKeyEditModel;
class frmEditTrade_State : public frmEditStateMap
{
    Q_OBJECT

public:
    frmEditTrade_State(const portfolio &portfolio_, QWidget *parent_ = 0);
    ~frmEditTrade_State();

    void enter();
    void leave();
    bool validate();

private slots:
    void customContextMenuRequested(const QPoint&);
    void listChange(const QModelIndex &current_, const QModelIndex&);
    void add();
    void remove();
    void tradeFrequencyChange(int index_);
    void tradeActionChange(int index_);
    void tradePriceChange(bool checked_);
    void securityFilterChange(int index_);

private:
    frmEditTrade_UI *ui;
    trade *m_currentItem;
    objectKeyEditModel *m_model;
    bool m_loading;

    void load();
    void save();

    QList<objectKeyBase*> sort(QList<objectKeyBase*> list_) const;
    static bool tradeSort(const objectKeyBase* const row1_, const objectKeyBase* const row2_);

    void validationError(const objectKeyBase &key_, const QString &errorMessage_);

    QString mimeType() const { return "application/mypersonalindex-trade"; }
    bool internalCopy(QDataStream &stream_) const;
    void internalPaste(QDataStream &stream_);
};

#endif // FRMEDITTRADE_STATE_H
