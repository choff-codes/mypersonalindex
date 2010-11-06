#ifndef MPITABLEVIEW_H
#define MPITABLEVIEW_H

#include <QObject>
#include <QTableView>

class mpiTableView : public QTableView
{
    Q_OBJECT

public:
    mpiTableView(bool hasRowLabels_, QWidget *parent_ = 0):
        QTableView(parent_),
        m_hasRowLabels(hasRowLabels_)
    {}

public slots:
    void exportTable(bool toClipboard_ = false);
    void copyTable() { exportTable(true); }

private:
    bool m_hasRowLabels;
};

#endif // MPITABLEVIEW_H
