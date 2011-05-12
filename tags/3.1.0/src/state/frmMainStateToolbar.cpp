#include "frmMainStateToolbar.h"
#include "frmMainToolbar_UI.h"
#include "tradeDateCalendar.h"

void frmMainStateToolbar::dateClicked()
{
    toolbarUI()->toolbarDateEndEdit->blockSignals(true);
    toolbarUI()->toolbarDateBeginEdit->blockSignals(true);

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    QDate endDate = QDate::fromJulianDay(m_portfolio.endDate());
    toolbarUI()->toolbarDateEndEdit->setDate(endDate);

    if (action == toolbarUI()->toolbar3M)
        toolbarUI()->toolbarDateBeginEdit->setDate(previousTradeDate(endDate.addMonths(-3)));
    else if (action == toolbarUI()->toolbar6M)
        toolbarUI()->toolbarDateBeginEdit->setDate(previousTradeDate(endDate.addMonths(-6)));
    else if (action == toolbarUI()->toolbar1Y)
        toolbarUI()->toolbarDateBeginEdit->setDate(previousTradeDate(endDate.addYears(-1)));
    else if (action == toolbarUI()->toolbarYTD)
        toolbarUI()->toolbarDateBeginEdit->setDate(previousTradeDate(QDate(endDate.year(), 1, 1)));
    else if (action == toolbarUI()->toolbar5Y)
        toolbarUI()->toolbarDateBeginEdit->setDate(previousTradeDate(endDate.addYears(-5)));
    else
        toolbarUI()->toolbarDateBeginEdit->setDate(QDate::fromJulianDay(m_portfolio.startDate()));


    toolbarUI()->toolbarDateEndEdit->blockSignals(false);
    toolbarUI()->toolbarDateBeginEdit->blockSignals(false);
    refreshTab();
}

QDate frmMainStateToolbar::previousTradeDate(const QDate &date_)
{
    return QDate::fromJulianDay(
        tradeDateCalendar::previousTradeDate(date_.toJulianDay())
    );
}
