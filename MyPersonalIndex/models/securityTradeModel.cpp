#include "securityTradeModel.h"

QVariant securityTradeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        trade t = m_list.at(index.row());
        switch(index.column())
        {
            case 0:
                return trade::tradeTypeToString(t.type);
                break;
            case 1:
                return trade::valueToString(t.type, t.value);
                break;
            case 2:
                return t.price < 0 ? "Prev Close" : functions::doubleToCurrency(t.price);
                break;
            case 3:
                return t.commission < 0 ? "" : functions::doubleToCurrency(t.commission);
                break;
            case 4:
                return m_cashAccounts.contains(t.cashAccount) ? m_cashAccounts.value(t.cashAccount).symbol : "";
                break;
            case 5:
                return trade::frequencyToString(t.frequency);
                break;
            case 6:
                return trade::dateToString(t.frequency, t.date);
                break;
            case 7:
                return t.startDate != 0 ? QDate::fromJulianDay(t.startDate).toString(Qt::SystemLocaleShortDate) : "";
                break;
            case 8:
                return t.endDate != 0 ? QDate::fromJulianDay(t.endDate).toString(Qt::SystemLocaleShortDate) : "";
                break;
        }
    }

    return QVariant();
}

QVariant securityTradeModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch(section)
    {
        case 0:
            return "Type";
            break;
        case 1:
            return "Value";
            break;
        case 2:
            return "Price";
            break;
        case 3:
            return "Comm.";
            break;
        case 4:
            return "$ Acct";
            break;
        case 5:
            return "Freq.";
            break;
        case 6:
            return "Date";
            break;
        case 7:
            return "Start";
            break;
        case 8:
            return "End";
            break;
    }

    return QVariant();
}

QString securityTradeModel::internalCopy(const trade &item)
{
    return QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9").arg(
           QString::number((int)item.type), functions::doubleToLocalFormat(item.value), functions::doubleToLocalFormat(item.price),
           functions::doubleToLocalFormat(item.commission), QString::number(item.cashAccount),
           QString::number((int)item.frequency), QString::number(item.date),
           QString::number(item.startDate), QString::number(item.endDate));
}

trade securityTradeModel::internalPaste(const QStringList &value, bool *ok)
{
    trade item;

    if (value.count() != 9)
    {
        (*ok) = false;
        return item;
    }

    int i = value.at(0).toInt(ok);
    if (*ok && i < trade::tradeType_Count)
        item.type = (trade::tradeType)i;

    if (*ok)
        item.value = value.at(1).toDouble(ok);

    if (*ok)
        item.price = value.at(2).toDouble(ok);

    if (*ok)
        item.commission = value.at(3).toDouble(ok);

    if (*ok)
        item.cashAccount = value.at(4).toInt(ok);

    if (*ok)
    {
        int i = value.at(5).toInt(ok);
        if (*ok && i < trade::tradeFreq_Count)
            item.frequency = (trade::tradeFreq)i;
    }

    if (*ok)
        item.date = value.at(6).toInt(ok);

    if (*ok)
        item.startDate = value.at(7).toInt(ok);

    if (*ok)
        item.endDate = value.at(8).toInt(ok);

    return item;
}

void securityTradeModel::autoResize()
{
    if (m_list.isEmpty())
        m_parent->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    else
        m_parent->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
