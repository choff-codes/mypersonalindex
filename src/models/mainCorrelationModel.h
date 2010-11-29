#ifndef MAINCORRELATIONMODEL_H
#define MAINCORRELATIONMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class mainCorrelationModel: public QAbstractTableModel
{
public:

    typedef QList<QList<double> > correlationMatrix;

    mainCorrelationModel(const QStringList &rows_, const correlationMatrix &correlationMatrix_, QObject *parent_ = 0):
        QAbstractTableModel(parent_),
        m_rows(rows_),
        m_correlationMatrix(correlationMatrix_)
    {
        insertRows(0, m_rows.count());
    }

    int rowCount(const QModelIndex&) const { return m_rows.count(); }
    int columnCount (const QModelIndex&) const { return m_rows.count(); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QStringList m_rows;
    correlationMatrix m_correlationMatrix;
};

#endif // MAINCORRELATIONMODEL_H
