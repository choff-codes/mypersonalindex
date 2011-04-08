#include "mainTradeModel.h"
#include "portfolio.h"
#include "executedTrade.h"

mainTradeModel::mainTradeModel(const portfolio &portfolio_, int beginDate_, int endDate_, QObject *parent_):
    QAbstractItemModel(parent_),
    m_securities(portfolio_.securities().values()),
    m_beginDate(beginDate_),
    m_endDate(endDate_)
{
    qStableSort(m_securities.begin(), m_securities.end(), securitySort);
}

bool mainTradeModel::securitySort(const security &row1_, const security &row2_)
{
    return row1_.displayText().toLower() < row2_.displayText().toLower();
}

QModelIndex mainTradeModel::index(int row_, int column_, const QModelIndex &parent_) const
{
    if (!hasIndex(row_, column_, parent_))
        return QModelIndex();

    if (!parent_.isValid())
        return createIndex(row_, column_, (-1 * row_) - 1);

    return createIndex(row_, column_, (int)(-1 * parent_.internalId()));
}

QModelIndex mainTradeModel::parent(const QModelIndex &index_) const
 {
    if (!index_.isValid())
        return QModelIndex();

    if (index_.internalId() < 0)
         return QModelIndex();

    return createIndex(index_.internalId() - 1, 0, (int)(-1 * index_.internalId()));
}

int mainTradeModel::rowCount(const QModelIndex &parent_) const
{
    if (parent_.column() > 0)
        return 0;

    if (!parent_.isValid())
        return m_securities.count();

    if (parent_.internalId() > 0)
        return 0;

    int i = 0;
    executedTradeMap &map = m_securities.at(-1 * (parent_.internalId() + 1)).executedTrades();
    QMap<int, executedTrade>::const_iterator begin = map.lowerBound(m_beginDate);
    QMap<int, executedTrade>::const_iterator end = map.constEnd();
    while (begin != end) {
        if (begin.key() > m_endDate)
            break;
        ++i;
        ++begin;
    }
    return i;
}

int mainTradeModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant mainTradeModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (role_ != Qt::DisplayRole)
        return QVariant();

    if (index_.internalId() < 0)
        return m_securities.at(-1 * (index_.internalId() + 1)).displayText();

    // performance bottleneck! keeping for now to save memory, but can easily change to be faster
    QMap<int, executedTrade>::const_iterator row = m_securities.at(index_.internalId() - 1).executedTrades().lowerBound(m_beginDate) + index_.row();
    return row.value().displayText(row.key());
}

Qt::ItemFlags mainTradeModel::flags(const QModelIndex &index_) const
{
    if (!index_.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
