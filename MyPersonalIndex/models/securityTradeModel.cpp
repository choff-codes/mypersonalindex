#include "securityTradeModel.h"

QVariant securityTradeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_trades.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        trade t = m_trades.at(index.row());
        switch(index.column())
        {
            case 0:
                return trade::tradeTypeToString(t.type);
                break;
            case 1:
                return trade::valueToString(t.type, t.value);
                break;
            case 2:
                return t.type == trade::tradeType_Interest || t.type == trade::tradeType_InterestPercent ? "" :
                        t.price < 0 ? "Prev Close" : functions::doubleToCurrency(t.price);
                break;
            case 3:
                return t.commission <= 0 ? "" : functions::doubleToCurrency(t.commission);
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

void securityTradeModel::paste()
{
    bool *ok = new bool;
    int i = 1;

    m_parent->selectionModel()->clearSelection();
    foreach(QString s, QApplication::clipboard()->text().remove('\r').split('\n'))
    {
        (*ok) = false;
        trade item = internalPaste(s.split('\t'), ok);
        if (*ok)
        {
            beginInsertRows(QModelIndex(), m_trades.count(), m_trades.count());
            m_trades.append(item);
            endInsertRows();
            selectItem(index(m_trades.count()-1, 0));
        }
        else
            QMessageBox::critical(m_dialog, "Paste Error", QString("Invalid format on row %1! Fix the error and paste again.").arg(i));
        i++;
    }

    delete ok;
}

void securityTradeModel::copy()
{
    const QList<int> indexes = getSelectedRows();
    if(indexes.isEmpty())
        return;

    QStringList lines;
    foreach(const int &i, indexes)
        lines.append(internalCopy(m_trades.at(i)));

    if (lines.isEmpty())
        return;

    QApplication::clipboard()->setText(lines.join("\n"));
}

QString securityTradeModel::internalCopy(const trade &item)
{
    return QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9").arg(
           QString::number((int)item.type), functions::doubleToLocalFormat(item.value, 4), functions::doubleToLocalFormat(item.price, 4),
           functions::doubleToLocalFormat(item.commission, 4), QString::number(item.cashAccount),
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
        item.value = QLocale().toDouble(value.at(1), ok);

    if (*ok)
        item.price = QLocale().toDouble(value.at(2), ok);

    if (*ok)
        item.commission = QLocale().toDouble(value.at(3), ok);

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
    if (m_trades.isEmpty())
        m_parent->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    else
        m_parent->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void securityTradeModel::selectItem(const QModelIndex &index)
{
    m_parent->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void securityTradeModel::editSelected()
{
    QModelIndexList il = m_parent->selectionModel()->selectedRows();
    if(il.isEmpty())
        return;

    foreach(const QModelIndex &q, il)
    {
        int i = q.row();
        frmTrade f(m_cashAccounts, m_dialog, m_trades.at(i));

        if (f.exec())
        {
            trade item = f.getReturnValues();
            m_trades[i] = item;
            emit dataChanged(index(i, 0), index(i, m_columns - 1));
        }
        selectItem(q);
    }
     autoResize();
}

void securityTradeModel::addNew()
{
    frmTrade f(m_cashAccounts, m_dialog);

    if (f.exec())
    {
        beginInsertRows(QModelIndex(), m_trades.count(), m_trades.count());

        trade item = f.getReturnValues();
        m_trades.append(item);

        endInsertRows();
    }
     autoResize();
}

void securityTradeModel::deleteSelected()
{
    const QList<int> indexes = getSelectedRows();
    if(indexes.isEmpty())
        return;

    for(int i = indexes.count() - 1; i >= 0; --i)
    {
        beginRemoveRows(QModelIndex(), i, i);

        m_trades.removeAt(indexes.at(i));

        endRemoveRows();
    }

    m_parent->selectionModel()->clearSelection();
    autoResize();
}

QMap<int, trade> securityTradeModel::saveList(const QMap<int, trade> &originalValues, const int &parentID)
{
    QMap<int, trade> returnValues;

    for(int i = 0; i < m_trades.count(); ++i) // save all items
    {
        if (m_trades.at(i).id == -1 || originalValues.value(m_trades.at(i).id) != m_trades.at(i))
            m_trades[i].save(parentID);

        returnValues.insert(m_trades.at(i).id, m_trades.at(i));
    }

    foreach(const trade &item, originalValues) // delete items that have been removed
        if(!returnValues.contains(item.id))
            item.remove();

    return returnValues;
}

QList<int> securityTradeModel::getSelectedRows() const
{
    QModelIndexList model = m_parent->selectionModel()->selectedRows();
    QList<int> indexes;
    if (model.isEmpty())
        return indexes;

    foreach(const QModelIndex &q, model)
        indexes.append(q.row());
    qSort(indexes);

    return indexes;
}
