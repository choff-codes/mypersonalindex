using System;
using System.Collections.Generic;
using System.Data.SqlServerCe;
using System.Data.SqlTypes;
using System.Drawing;
using System.Windows.Forms;
using ZedGraph;

namespace MyPersonalIndex
{
    partial class frmMain
    {
        private void LoadAccounts(DateTime Date)
        {
            AvgPrice.GetAveragePricePerShare(Date, SQL, MPI.Portfolio.CostCalc, MPI.Portfolio.ID);
            MPI.Account.TotalValue = GetTotalValue(Date);
            dgAcct.DataSource = SQL.ExecuteDataset(MainQueries.GetAcct(MPI.Portfolio.ID, Date, MPI.Account.TotalValue, MPI.Account.Sort, btnAcctShowBlank.Checked));

            LoadGainLossInfo(dgAcct, Date, (int)Constants.MPIAccount.CostBasisColumn, (int)Constants.MPIAccount.GainLossColumn,
                (int)Constants.MPIAccount.TaxLiabilityColumn, true, true, true);
            dgAcct.Columns[Constants.MPIAccount.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.Account.TotalValue);
        }

        private void LoadAssetAllocation(DateTime Date)
        {
            MPI.AA.TotalValue = GetTotalValue(Date);
            dgAA.DataSource = SQL.ExecuteDataset(MainQueries.GetAA(MPI.Portfolio.ID, Date, MPI.AA.TotalValue, MPI.AA.Sort, btnAAShowBlank.Checked));

            dgAA.Columns[Constants.MPIAssetAllocation.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.AA.TotalValue);
        }

        private void LoadCorrelations(DateTime StartDate, DateTime EndDate)
        {
            dgCorrelation.Rows.Clear();
            dgCorrelation.Columns.Clear();

            try
            {
                this.Cursor = Cursors.WaitCursor;
                List<string> CorrelationItems = new List<string>();
                CorrelationItems.Add(Constants.SignifyPortfolio + MPI.Portfolio.ID.ToString());

                using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetCorrelationDistinctTickers(MPI.Portfolio.ID, btnCorrelationHidden.Checked)))
                    foreach (SqlCeUpdatableRecord rec in rs)
                        CorrelationItems.Add(rec.GetString((int)MainQueries.eGetCorrelationDistinctTickers.Ticker));

                foreach (string s in CorrelationItems)
                    dgCorrelation.Columns.Add(s, s);
                foreach (DataGridViewColumn d in dgCorrelation.Columns)
                    d.SortMode = DataGridViewColumnSortMode.NotSortable;

                dgCorrelation.Rows.Add(CorrelationItems.Count);
                for (int i = 0; i < CorrelationItems.Count; i++)
                    dgCorrelation.Rows[i].HeaderCell.Value = CorrelationItems[i];

                // replace portfolio ID with portfolio name
                dgCorrelation.Rows[0].HeaderCell.Value = MPI.Portfolio.Name;
                dgCorrelation.Columns[0].HeaderCell.Value = MPI.Portfolio.Name;

                for (int i = 0; i < CorrelationItems.Count; i++)
                    for (int x = i; x < CorrelationItems.Count; x++)
                        if (x == i)
                            dgCorrelation[i, x].Value = 100.0;
                        else
                        {
                            try
                            {
                                dgCorrelation[i, x].Value = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetCorrelation(CorrelationItems[i], CorrelationItems[x], StartDate, EndDate), 0));
                                dgCorrelation[x, i].Value = dgCorrelation[i, x].Value;
                            }
                            catch (SqlCeException)
                            {
                                dgCorrelation[i, x].Value = 0;
                                dgCorrelation[x, i].Value = 0;
                            }
                        }
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }

        private void LoadGainLossInfo(DataGridView dg, DateTime Date, int CostBasisCol, int GainLossCol, int TaxCol, bool bCostBasis, bool bGainLoss, bool bTax)
        {
            double CostBasis = 0;
            double GainLoss = 0;
            double TaxLiability = 0;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetGainLossInfo(MPI.Portfolio.ID, Date)))
                if (rs.HasRows)
                {
                    rs.ReadFirst();
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.CostBasis)))
                        CostBasis = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.CostBasis);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.GainLoss)))
                        GainLoss = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.GainLoss);
                    if (!Convert.IsDBNull(rs.GetValue((int)MainQueries.eGetGainLossInfo.TaxLiability)))
                        TaxLiability = (double)rs.GetDecimal((int)MainQueries.eGetGainLossInfo.TaxLiability);
                }

            if (bCostBasis)
                dg.Columns[CostBasisCol].HeaderCell.Value = "Cost Basis (" + String.Format("{0:C})", CostBasis);

            if (bTax)
                dg.Columns[TaxCol].HeaderCell.Value = "Tax Liability (" + String.Format("{0:C})", TaxLiability);

            if (bGainLoss)
            {
                string sGainLoss = String.Format("{0:C})", GainLoss);
                if (GainLoss < 0)
                    sGainLoss = "-" + sGainLoss.Replace("(", String.Empty).Replace(")", String.Empty) + ")";
                dg.Columns[GainLossCol].HeaderCell.Value = "Gain/Loss (" + sGainLoss;
            }
        }

        private void LoadGraph(DateTime StartDate, DateTime EndDate)
        {
            GraphPane g = zedChart.GraphPane;
            PointPairList list = new PointPairList();

            Functions.LoadGraphSettings(zedChart, MPI.Portfolio.Name, false);

            DateTime YDay = Convert.ToDateTime(SQL.ExecuteScalar(MainQueries.GetPreviousDay(StartDate), SqlDateTime.MinValue.Value));
            if (YDay == SqlDateTime.MinValue.Value)
                return;

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetChart(MPI.Portfolio.ID, Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetNAV(MPI.Portfolio.ID, YDay))), StartDate, EndDate)))
                if (rs.HasRows)
                {
                    list.Add(new XDate(YDay), 0);

                    foreach (SqlCeUpdatableRecord rec in rs)
                        list.Add(new XDate(rec.GetDateTime((int)MainQueries.eGetChart.Date)), (double)rec.GetDecimal((int)MainQueries.eGetChart.Gain));

                    LineItem line = g.AddCurve(String.Empty, list, Color.Crimson, SymbolType.None);
                    line.Line.Width = 3;

                    g.XAxis.Scale.Min = list[0].X;
                    g.XAxis.Scale.Max = list[list.Count - 1].X;
                }

            zedChart.AxisChange();
            zedChart.Refresh();
        }

        private void LoadHoldings(DateTime Date)
        {
            AvgPrice.GetAveragePricePerShare(Date, SQL, MPI.Portfolio.CostCalc, MPI.Portfolio.ID);

            MPI.Holdings.TotalValue = GetTotalValue(Date);
            dgHoldings.DataSource = SQL.ExecuteDataset(MainQueries.GetHoldings(MPI.Portfolio.ID, Date, MPI.Holdings.TotalValue, btnHoldingsHidden.Checked, MPI.Holdings.Sort));

            LoadGainLossInfo(dgHoldings, Date, (int)Constants.MPIHoldings.CostBasisColumn, (int)Constants.MPIHoldings.GainLossColumn,
                0, true, true, false);
            dgHoldings.Columns[Constants.MPIHoldings.TotalValueColumn].HeaderCell.Value = "Total Value (" + String.Format("{0:C})", MPI.Holdings.TotalValue);
        }

        private void LoadNAV()
        {
            dgPerformance.DataSource = SQL.ExecuteDataset(MainQueries.GetAllNav(MPI.Portfolio.ID, MPI.Portfolio.NAVStart, btnPerformanceSortDesc.Checked));
        }

        private void LoadStat(DateTime StartDate, DateTime EndDate, bool DateChange)
        {
            AvgPrice.GetAveragePricePerShare(EndDate, SQL, MPI.Portfolio.CostCalc, MPI.Portfolio.ID);

            if (!DateChange)
                dgStats.Rows.Clear();
            MPI.Stat.TotalValue = GetTotalValue(EndDate);

            int i = -1;
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetStats(MPI.Portfolio.ID)))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    if (!DateChange)
                        i = dgStats.Rows.Add();
                    else
                        i++;

                    try
                    {
                        dgStats.Rows[i].HeaderCell.Value = rec.GetString((int)MainQueries.eGetStats.Description);
                        dgStats[0, i].Value = Functions.FormatStatString(SQL.ExecuteScalar(CleanStatString(rec.GetString((int)MainQueries.eGetStats.SQL))),
                            (Constants.OutputFormat)rec.GetInt32((int)MainQueries.eGetStats.Format));
                    }
                    catch (SqlCeException)
                    {
                        dgStats[0, i].Value = "Error";
                    }
                    catch (ArgumentOutOfRangeException)
                    {
                        dgStats[0, i].Value = "Error";
                    }
                }
        }
    }
}