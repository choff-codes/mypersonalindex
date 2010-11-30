#include "mainCorrelationModel.h"
#include <QColor>
#include <QVector>
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

correlationRow::correlationRow(objectType type_, int id_, const QString &description_, const QMap<correlationRow, double> correlationValues_):
    baseRow(correlationOrder),
    correlationValues(correlationValues_)
{
//    row_ObjectType,
    values.append((int)type_);
//    row_ID,
    values.append(id_);
//    row_Description
    values.append(description_);
}

bool correlationRow::operator==(const correlationRow &other_) const
{
    return values.at(row_ObjectType).toInt() == other_.values.at(row_ObjectType).toInt()
        && values.at(row_ID).toInt() == other_.values.at(row_ID).toInt();
}

bool correlationRow::operator<(const correlationRow &other_) const
{
    if (values.at(row_ObjectType).toInt() < other_.values.at(row_ObjectType).toInt())
        return true;

    return values.at(row_ID).toInt() < other_.values.at(row_ID).toInt();
}

mainCorrelationModel::mainCorrelationModel(const QList<baseRow*> &rows_, QObject *parent_):
    mpiViewModelBase(rows_, QVector<int>(rows_.size(), 0).toList(), parent_)
{
}

QVariant mainCorrelationModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (role_ != Qt::DisplayRole && role_ != Qt::BackgroundRole)
        return QVariant();

    double value = 1;
    if (index_.row() != index_.column())
        value = static_cast<correlationRow*>(m_rows.at(index_.row()))->correlationValues.value(*static_cast<correlationRow*>(m_rows.at(index_.column())));

    if(role_ == Qt::DisplayRole)
        return functions::doubleToPercentage(value);

    // Qt::BackgroundRole
    value *= 127;
    if (value < 0) // Background Role
        return QColor((int)(255 + value), 255, (int)(255 + value));
    else
        return QColor(255, (int)(255 - value), (int)(255 - value));
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
    foreach(baseRow *row, m_rows)
        static_cast<correlationRow*>(row)->correlationValues.insert(key_, row_->correlationValues.value(*static_cast<correlationRow*>(row)));
    m_rows.append(row_);
    endInsertRows();

    beginInsertColumns(QModelIndex(), m_rows.count(), m_rows.count());
    m_viewableColumns.append(0);
    endInsertColumns();

    sortColumns();
    emit dataChanged(index(0, 0), index(m_rows.count() - 1, m_viewableColumns.count() - 1));
}

void mainCorrelationModel::remove(const correlationRow &key_)
{
    int index = -1;
    for(int i = 0; i < m_rows.count(); ++i)
        if (*static_cast<correlationRow*>(m_rows.at(i)) == key_)
        {
            index = i;
            break;
        }

    if (index == -1)
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_rows.removeAt(index);
    endRemoveRows();

    beginRemoveColumns(QModelIndex(), index, index);
    m_viewableColumns.removeLast();
    endInsertColumns();
}
