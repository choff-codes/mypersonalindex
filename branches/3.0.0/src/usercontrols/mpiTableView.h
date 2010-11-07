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
    void exportTable();
    void copyTable();

private:  
    bool m_hasRowLabels;

    QStringList formatTableForExport(const QList<int> &rows_, const QString &delimiter_);
    QString formatHeaderForExport(const QString &delimiter_);
};

#endif // MPITABLEVIEW_H
