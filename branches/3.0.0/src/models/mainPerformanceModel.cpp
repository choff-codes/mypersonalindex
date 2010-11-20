#include "mainPerformanceModel.h"
#include <QStringList>
#include "historicalNAV.h"
#include "tradeDateCalendar.h"
#include "functions.h"

//enum {
//    row_Date,
//    row_TotalValue,
//    row_Dividend,
//    row_Index,
//    row_Change,
//    row_Gain
//};

const QStringList performanceRow::columns = QStringList()
                                            << "Date"
                                            << "Total Value"
                                            << "Dividends"
                                            << "Index"
                                            << "% Change"
                                            << "% Gain";

const QVariantList performanceRow::columnsType = QVariantList()
                                                 << QVariant(QVariant::Int)
                                                 << QVariant(QVariant::Double)
                                                 << QVariant(QVariant::Double)
                                                 << QVariant(QVariant::Double)
                                                 << QVariant(QVariant::Double)
                                                 << QVariant(QVariant::Double);

performanceRow::performanceRow(int date_, double totalValue_, double dividend_, double index_, double change_, double gain_, const QList<orderBy> &columnSort_):
    baseRow(columnSort_)
{
    //    row_Date,
    this->values.append(date_);
    //    row_TotalValue,
    this->values.append(totalValue_);
    //    row_Dividend,
    this->values.append(dividend_);
    //    row_Index,
    this->values.append(index_);
    //    row_Change,
    this->values.append(change_);
    //    row_Gain
    this->values.append(gain_);
}

QList<baseRow*> performanceRow::getRows(const historicalNAV &nav_, const QList<orderBy> &columnSort_)
{
    if (nav_.isEmpty())
        return QList<baseRow*>();

    QList<baseRow*> returnList;
    tradeDateCalendar calendar(nav_.beginDate());
    int endDate = nav_.endDate();

    navPair previousNAV = nav_.value(calendar.date());
    double beginNAV = previousNAV.nav;

    foreach(const int &date, calendar)
    {
        navPair nav = nav_.value(date);
        returnList.append(new performanceRow(date, nav.totalValue, nav.dividend, nav.nav, (nav.nav - previousNAV.nav) / previousNAV.nav, (nav.nav - beginNAV) / beginNAV, columnSort_));

        if (date == endDate)
            break;

        previousNAV = nav;
    }

    return returnList;
}

QMap<int, QString> performanceRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = functions::removeNewLines(columns.at(i));

    return names;
}


mainPerformanceModel::mainPerformanceModel(const QList<baseRow*> &rows_, const QList<int> &viewableColumns_, QObject *parent_ ):
    mpiViewModelBase(rows_, viewableColumns_, parent_)
{
}

QVariant mainPerformanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    int column = m_viewableColumns.at(index.column());
    QVariant value = m_rows.at(index.row())->values.at(column);

    switch (column)
    {
        case performanceRow::row_Date:
            return QDate::fromJulianDay(value.toInt()).toString(Qt::SystemLocaleShortDate);
        case performanceRow::row_TotalValue:
        case performanceRow::row_Dividend:
        case performanceRow::row_Index:
            return functions::doubleToLocalFormat(value.toDouble());
        case performanceRow::row_Change:
        case performanceRow::row_Gain:
            return functions::doubleToPercentage(value.toDouble());
    };

    return QVariant();
}

QVariant mainPerformanceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (section >= m_viewableColumns.count())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return (int)Qt::AlignLeft | Qt::AlignVCenter;

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    return performanceRow::columns.at(m_viewableColumns.at(section));
}
