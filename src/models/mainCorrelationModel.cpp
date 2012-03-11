#include "mainCorrelationModel.h"
#include <QColor>
#include <QVector>
#include <QPalette>
#include "functions.h"

const QList<orderBy> correlationRow::correlationOrder = QList<orderBy>()
                                                        << orderBy(correlationRow::row_ObjectType, orderBy::order_ascending)
                                                        << orderBy(correlationRow::row_Description, orderBy::order_ascending);

//enum {
//    row_ObjectType,
//    row_ID,
//    row_Description
//};

const QVariantList correlationRow::columnsType = QVariantList()
                                                 << QVariant(QVariant::Int)
                                                 << QVariant(QVariant::Int)
                                                 << QVariant(QVariant::String);

correlationRow::correlationRow(int type_, int id_, const QString &description_):
    baseRow(correlationOrder)
{
    //    row_ObjectType,
    values.append(type_);
    //    row_ID,
    values.append(id_);
    //    row_Description
    values.append(description_);
}

correlationRow::correlationRow(int type_, int id_, const QString &description_, const QMap<correlationRow, double> &correlationValues_):
    baseRow(correlationOrder),
    correlationValues(correlationValues_)
{
    //    row_ObjectType,
    values.append(type_);
    //    row_ID,
    values.append(id_);
    //    row_Description
    values.append(description_);
}

bool correlationRow::operator==(const correlationRow &other_) const
{
    return values.at(row_ObjectType).toInt() == other_.values.at(row_ObjectType).toInt()
        && values.at(row_ID).toInt() == other_.values.at(row_ID).toInt()
        && values.at(row_Description).toString() == other_.values.at(row_Description).toString();
}

bool correlationRow::operator<(const correlationRow &other_) const
{
    if (values.at(row_ObjectType).toInt() < other_.values.at(row_ObjectType).toInt())
        return true;

    if (values.at(row_ID).toInt() < other_.values.at(row_ID).toInt())
        return true;

    return values.at(row_Description).toString() < other_.values.at(row_Description).toString();
}

mainCorrelationModel::mainCorrelationModel(const QList<baseRow*> &rows_, QObject *parent_):
    mpiViewModelBase(rows_, QVector<int>(rows_.count(), 0).toList(), false, parent_)
{
}

QVariant mainCorrelationModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (role_ == Qt::ForegroundRole)
        return QColor(Qt::black);

    if (role_ != Qt::DisplayRole && role_ != Qt::BackgroundRole)
        return QVariant();

    double value = 1;
    if (index_.row() != index_.column())
        value = static_cast<correlationRow*>(m_rows.at(index_.row()))->correlationValues.value(*static_cast<correlationRow*>(m_rows.at(index_.column())));

    if(role_ == Qt::DisplayRole)
        return functions::doubleToPercentage(value);

    //Qt::BackgroundRole

    value *= 200;

    if (value < 0)
        return QColor(255 + value, 255, 255 + value);
    else
        return QColor(255, 255 - value, 255 - value);

    return QVariant();
}

QVariant mainCorrelationModel::headerData(int section_, Qt::Orientation /* not used */, int role_) const
{
    if (role_ != Qt::DisplayRole)
        return QVariant();

    return m_rows.at(section_)->values.at(correlationRow::row_Description);
}


void mainCorrelationModel::add(correlationRow *row_, const correlationRow &key_)
{
    beginInsertRows(QModelIndex(), m_rows.count(), m_rows.count());
    // add correlation to every other row, which is calculated and passed in with the row_ object
    foreach(baseRow *existingRow, m_rows)
        static_cast<correlationRow*>(existingRow)->correlationValues.insert(key_, row_->correlationValues.value(*static_cast<correlationRow*>(existingRow)));
    m_rows.append(row_);
    endInsertRows();

    beginInsertColumns(QModelIndex(), m_rows.count(), m_rows.count());
    // add dummy column
    m_viewableColumns.append(0);
    endInsertColumns();

    sortRows();
    emit dataChanged(index(0, 0), index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1));
}

void mainCorrelationModel::remove(const correlationRow &key_)
{
    int x = -1;
    for(int i = 0; i < m_rows.count(); ++i)
        if (*static_cast<correlationRow*>(m_rows.at(i)) == key_)
        {
            x = i;
            break;
        }

    if (x == -1 || x >= m_rows.count() || m_viewableColumns.isEmpty())
        return;

    // remove row and column at the same time to prevent out of index exceptions
    beginRemoveRows(QModelIndex(), x, x);
    beginRemoveColumns(QModelIndex(), x, x);
    m_rows.removeAt(x);
    m_viewableColumns.removeLast();
    endRemoveRows();
    endRemoveColumns();
}
