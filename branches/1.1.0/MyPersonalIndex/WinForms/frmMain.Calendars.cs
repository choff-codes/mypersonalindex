using System;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    partial class frmMain
    {

        /************************* Date functions ***********************************/

        private DateTime GetCurrentDateOrPrevious(DateTime d)
        {
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrPrevious(d), MPI.Portfolio.StartDate));
        }

        private DateTime GetCurrentDateOrNext(DateTime d)
        {
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(d), MPI.LastDate));
        }

        private DateTime GetCurrentDateOrNext(DateTime d, DateTime defaultValue)
        {
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(d), defaultValue));
        }

        private DateTime CheckPortfolioStartDate(DateTime StartDate)
        {
            // if start date is not a market day, find the next day
            StartDate = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetCurrentDayOrNext(StartDate), StartDate));

            // if there is a day before, return successfully
            // otherwise, there needs to be 1 day before to pull previous day closing prices
            if (Convert.ToInt32(SQL.ExecuteScalar(MainQueries.GetDaysNowAndBefore(StartDate))) >= 2)
                return StartDate;

            // recalculate portfolio from the start of the 2nd day of pricing
            return Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetSecondDay(), MPI.LastDate < StartDate ? StartDate : MPI.LastDate));
        }

        /************************* Reset Calendars ***********************************/

        private void ResetCalendars()
        {
            ResetCalendar(MPI.AA.Calendar, btnAADate, out MPI.AA.SelDate);
            ResetCalendar(MPI.Account.Calendar, btnAcctDate, out MPI.Account.SelDate);
            ResetCalendar(MPI.Holdings.Calendar, btnHoldingsDate, out MPI.Holdings.SelDate);
            ResetCalendar(MPI.Correlation.CalendarBegin, MPI.Correlation.CalendarEnd, btnCorrelationStartDate,
                btnCorrelationEndDate, out MPI.Correlation.BeginDate, out MPI.Correlation.EndDate);
            ResetCalendar(MPI.Chart.CalendarBegin, MPI.Chart.CalendarEnd, btnChartStartDate,
                btnChartEndDate, out MPI.Chart.BeginDate, out MPI.Chart.EndDate);
            ResetCalendar(MPI.Stat.CalendarBegin, MPI.Stat.CalendarEnd, btnStatStartDate,
                btnStatEndDate, out MPI.Stat.BeginDate, out MPI.Stat.EndDate);
        }

        private void ResetCalendar(MonthCalendar m, ToolStripDropDownButton t, out DateTime d)
        {
            d = MPI.LastDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MPI.LastDate;
            m.MinDate = MPI.Portfolio.StartDate;
            m.SetDate(d);
            t.Text = "Date: " + d.ToShortDateString();
        }

        private void ResetCalendar(MonthCalendar m1, MonthCalendar m2, ToolStripDropDownButton t1, ToolStripDropDownButton t2, out DateTime d1, out DateTime d2)
        {
            d2 = MPI.LastDate < MPI.Portfolio.StartDate ? MPI.Portfolio.StartDate : MPI.LastDate;
            d1 = MPI.Portfolio.StartDate;
            m1.MinDate = MPI.Portfolio.StartDate;
            m1.SetDate(MPI.Portfolio.StartDate);
            t1.Text = "Start Date: " + MPI.Portfolio.StartDate.ToShortDateString();
            m2.MinDate = MPI.Portfolio.StartDate;
            m2.SetDate(d2);
            t2.Text = "End Date: " + d2.ToShortDateString();
        }

        /************************* Calendar date change ***********************************/
        private void Date_Change(object sender, DateRangeEventArgs e)
        {
            if (sender == MPI.Holdings.Calendar)
            {
                MPI.Holdings.SelDate = GetCurrentDateOrPrevious(MPI.Holdings.Calendar.SelectionStart);
                btnHoldingsDate.HideDropDown();
                btnHoldingsDate.Text = string.Format("Date: {0}", MPI.Holdings.SelDate.ToShortDateString());
                LoadHoldings(MPI.Holdings.SelDate);
            }
            else if (sender == MPI.AA.Calendar)
            {
                MPI.AA.SelDate = GetCurrentDateOrPrevious(MPI.AA.Calendar.SelectionStart);
                btnAADate.HideDropDown();
                btnAADate.Text = string.Format("Date: {0}", MPI.AA.SelDate.ToShortDateString());
                LoadAssetAllocation(MPI.AA.SelDate);
            }
            else if (sender == MPI.Account.Calendar)
            {
                MPI.Account.SelDate = GetCurrentDateOrPrevious(MPI.Account.Calendar.SelectionStart);
                btnAcctDate.HideDropDown();
                btnAcctDate.Text = string.Format("Date: {0}", MPI.Account.SelDate.ToShortDateString());
                LoadAccounts(MPI.Account.SelDate);
            }
            else if (sender == MPI.Chart.CalendarBegin || sender == MPI.Chart.CalendarEnd)
            {
                MPI.Chart.BeginDate = GetCurrentDateOrNext(MPI.Chart.CalendarBegin.SelectionStart);
                MPI.Chart.EndDate = GetCurrentDateOrPrevious(MPI.Chart.CalendarEnd.SelectionStart);
                btnChartStartDate.HideDropDown();
                btnChartEndDate.HideDropDown();
                btnChartStartDate.Text = string.Format("Start Date: {0}", MPI.Chart.BeginDate.ToShortDateString());
                btnChartEndDate.Text = string.Format("End Date: {0}", MPI.Chart.EndDate.ToShortDateString());
                LoadGraph(MPI.Chart.BeginDate, MPI.Chart.EndDate);
            }
            else if (sender == MPI.Correlation.CalendarBegin || sender == MPI.Correlation.CalendarEnd)
            {
                MPI.Correlation.BeginDate = GetCurrentDateOrNext(MPI.Correlation.CalendarBegin.SelectionStart);
                MPI.Correlation.EndDate = GetCurrentDateOrPrevious(MPI.Correlation.CalendarEnd.SelectionStart);
                btnCorrelationStartDate.HideDropDown();
                btnCorrelationEndDate.HideDropDown();
                btnCorrelationStartDate.Text = string.Format("Start Date: {0}", MPI.Correlation.BeginDate.ToShortDateString());
                btnCorrelationEndDate.Text = string.Format("End Date: {0}", MPI.Correlation.EndDate.ToShortDateString());
            }
            else if (sender == MPI.Stat.CalendarBegin || sender == MPI.Stat.CalendarEnd)
            {
                MPI.Stat.BeginDate = GetCurrentDateOrNext(MPI.Stat.CalendarBegin.SelectionStart);
                MPI.Stat.EndDate = GetCurrentDateOrPrevious(MPI.Stat.CalendarEnd.SelectionStart);
                btnStatStartDate.HideDropDown();
                btnStatEndDate.HideDropDown();
                btnStatStartDate.Text = string.Format("Start Date: {0}", MPI.Stat.BeginDate.ToShortDateString());
                btnStatEndDate.Text = string.Format("End Date: {0}", MPI.Stat.EndDate.ToShortDateString());
                LoadStat(MPI.Stat.BeginDate, MPI.Stat.EndDate, true);
            }
        }
    }
}