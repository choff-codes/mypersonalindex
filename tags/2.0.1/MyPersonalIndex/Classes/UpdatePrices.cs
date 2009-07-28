using System;
using System.Collections.Generic;
using System.Data.SqlServerCe;
using System.IO;
using System.Net;
using System.Windows.Forms;

namespace MyPersonalIndex
{
    class UpdatePrices : IDisposable
    {
        private MainQueries SQL;
        private Constants.MPISettings Settings;
        private MPIBackgroundWorker bw;
        private DateTime LastDate;

        public UpdatePrices(MainQueries SQL, Constants.MPISettings Settings, MPIBackgroundWorker bw, DateTime LastDate)
        {
            this.SQL = SQL;
            this.Settings = Settings;
            this.bw = bw;
            this.LastDate = LastDate;
        }

        public void Dispose()
        {
            SQL = null;
            Settings = null;
            bw = null;
        }

        /************************* Start update of prices ***********************************/

        public void UpdateAllPrices()
        {
            Dictionary<string, Constants.UpdateInfo> Tickers = GetTickerUpdateList();
            DateTime MinDate;
            List<string> TickersNotUpdated = new List<string>();

            if (Tickers.Count == 0)
                return;

            if (!Functions.IsInternetConnection())
            {
                MessageBox.Show("No Internet connection!", "Connection", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            GetTickerUpdateInfo(Tickers);
            MinDate = DateTime.Today.AddDays(1); // arbitrary value

            foreach (KeyValuePair<string, Constants.UpdateInfo> i in Tickers)
            {
                try
                {
                    // Prices are the last thing updated, so it is possible to download dividends/splits for a day and not prices
                    // Therefore, a check needs to be added to prevent duplicate dividends/splits
                    GetDividends(i.Key, i.Value.DividendDate > i.Value.ClosingDate ? i.Value.DividendDate : i.Value.ClosingDate);
                    if (Settings.Splits)
                        GetSplits(i.Key, i.Value.SplitDate > i.Value.ClosingDate ? i.Value.SplitDate : i.Value.ClosingDate);
                    GetPrices(i.Key, i.Value.ClosingDate, i.Value.Price);

                    // Set this here so if a ticker is not updated, mindate is not effected
                    // ClosingDate will always be the earliest, not dividend or split dates
                    if (i.Value.ClosingDate < MinDate)
                        MinDate = i.Value.ClosingDate;
                }
                catch (WebException)
                {
                    TickersNotUpdated.Add(i.Key);
                }
            }

            InsertMissingPrices(); // fill in any prices Yahoo! finance is missing equal to the previous close (0% change)
            SQL.ExecuteNonQuery(MainQueries.InsertCashPrices()); // update the cash $1 position to fill in for new dates

            using (NAV n = new NAV(SQL, bw, LastDate))
                n.GetNAV(-1, MinDate); // update all portfolios

            if (TickersNotUpdated.Count != 0)
                MessageBox.Show("The following tickers were not updated (Yahoo! Finance may not yet have today's price):\n\n" + string.Join(", ", TickersNotUpdated.ToArray()));
        }

        private Dictionary<string, Constants.UpdateInfo> GetTickerUpdateList()
        {
            Dictionary<string, Constants.UpdateInfo> Tickers = new Dictionary<string, Constants.UpdateInfo>();
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetUpdateDistinctTickers()))
                foreach (SqlCeUpdatableRecord rec in rs)
                    // default to 6 days of data before the data start date, this ensures we have at least a day or two of pervious closing prices
                    // the date will be updated later on if it already exists in ClosingPrices
                    Tickers.Add(rec.GetString((int)MainQueries.eGetUpdateDistinctTickers.Ticker), new Constants.UpdateInfo(Settings.DataStartDate.AddDays(-6)));

            return Tickers;
        }

        private void GetTickerUpdateInfo(Dictionary<string, Constants.UpdateInfo> Tickers)
        {
            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetUpdateLastRunDates()))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    Constants.UpdateInfo Info = Tickers[rec.GetString((int)MainQueries.eGetUpdateLastRunDates.Ticker)];
                    switch (rec.GetString((int)MainQueries.eGetUpdateLastRunDates.Type))
                    {
                        case "C":
                            Info.ClosingDate = rec.GetDateTime((int)MainQueries.eGetUpdateLastRunDates.Date);
                            Info.Price = (double)rec.GetDecimal((int)MainQueries.eGetUpdateLastRunDates.Price);
                            break;
                        case "D":
                            Info.DividendDate = rec.GetDateTime((int)MainQueries.eGetUpdateLastRunDates.Date);
                            break;
                        case "S":
                            Info.SplitDate = rec.GetDateTime((int)MainQueries.eGetUpdateLastRunDates.Date);
                            break;
                    }
                }
        }

        /************************* Download closing prices, splits, and dividends ***********************************/

        private void GetPrices(string Ticker, DateTime MinDate, double LastPrice)
        {
            // Yahoo! Finance will always use US format
            System.Globalization.CultureInfo C = System.Globalization.CultureInfo.InvariantCulture;

            if (MinDate == DateTime.Today)
                return;

            WebClient Client = new WebClient();
            string line;
            string[] columns;

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.ClosingPrices))
            // add a day to mindate since we already have data for mindate
            using (Stream s = Client.OpenRead(MainQueries.GetCSVAddress(Ticker, MinDate.AddDays(1), DateTime.Now, Constants.StockPrices)))
            using (StreamReader sr = new StreamReader(s))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                sr.ReadLine();  // first row is header
                line = sr.ReadLine();

                if (line == null)
                    return;

                columns = line.Split(',');
                // columns[0] = Date
                // columns[4] = Close Price
                do
                {
                    DateTime Day = Convert.ToDateTime(columns[0], C);
                    double Price = Convert.ToDouble(columns[4], C);

                    newRecord.SetDateTime((int)MainQueries.Tables.eClosingPrices.Date, Day);
                    newRecord.SetString((int)MainQueries.Tables.eClosingPrices.Ticker, Ticker);
                    newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Price, (decimal)Price);

                    double Split = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetSplit(Ticker, Day), 1));
                    double Div = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetDividend(Ticker, Day), 0));
                    Price = (Price * Split) + Div;

                    line = sr.ReadLine(); // data is sorted by date descending, get next row to calculate change

                    // data is in descending date order, so get the next row to calculate % change
                    if (line != null)
                    {
                        columns = line.Split(',');
                        newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Change, (decimal)(((Price / Convert.ToDouble(columns[4], C)) - 1) * 100));
                    }
                    else // use last price that is passed in as a parameter
                    {
                        if (LastPrice == 0) // no previous day, so it's the first closing price
                            newRecord.SetValue((int)MainQueries.Tables.eClosingPrices.Change, System.DBNull.Value);
                        else
                            newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Change, (decimal)(((Price / LastPrice) - 1) * 100));
                    }

                    rs.Insert(newRecord);
                }
                while (line != null);
            }
        }

        private void GetSplits(string Ticker, DateTime MinDate)
        {
            // Yahoo! Finance will always use US format
            System.Globalization.CultureInfo C = System.Globalization.CultureInfo.InvariantCulture;

            if (MinDate == DateTime.Today)
                return;

            WebClient Client = new WebClient();
            string line;
            string HTMLSplitStart = "<center>Splits:<nobr>";  // text starting splits
            string HTMLSplitNone = "<br><center>Splits:none</center>"; // same line, but signifying no splits

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Splits))
            using (Stream s = Client.OpenRead(MainQueries.GetSplitAddress(Ticker)))
            using (StreamReader sr = new StreamReader(s))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();

                do
                {
                    line = sr.ReadLine();
                    if (line == null)
                        break;
                }
                while (!line.Contains(HTMLSplitStart) && !line.Contains(HTMLSplitNone));

                if (line == null || line.Contains(HTMLSplitNone)) // no splits
                    return;

                int i = line.IndexOf(HTMLSplitStart) + HTMLSplitStart.Length;
                line = line.Substring(i, line.IndexOf("</center>", i) - i); // read up to </center> tag
                string[] splits = line.Split(new string[1] { "</nobr>, <nobr>" }, StringSplitOptions.None);
                //the last split is missing the ", <nobr>", so we have to strip off the </nobr>"
                splits[splits.Length - 1] = splits[splits.Length - 1].Replace("</nobr>", String.Empty);

                foreach (string p in splits)
                {
                    // split[0] = date of split
                    // split[1] = ratio
                    string[] split = p.Split(' ');
                    // if split is less than mindate, don't add unless this is the first time run for the ticker
                    if (Convert.ToDateTime(split[0], C) <= MinDate && MinDate >= Settings.DataStartDate)
                        continue;

                    // ratio looks like [2:1], so strip off the brackets
                    split[1] = split[1].Substring(1, split[1].Length - 2);
                    string[] divisor = split[1].Split(':');

                    newRecord.SetString((int)MainQueries.Tables.eSplits.Ticker, Ticker);
                    newRecord.SetDateTime((int)MainQueries.Tables.eSplits.Date, Convert.ToDateTime(split[0], C));
                    newRecord.SetDecimal((int)MainQueries.Tables.eSplits.Ratio, Convert.ToDecimal(divisor[0], C) / Convert.ToDecimal(divisor[1], C));
                    rs.Insert(newRecord);
                }
            }
        }

        private void GetDividends(string Ticker, DateTime MinDate)
        {
            // Yahoo! Finance will always use US format
            System.Globalization.CultureInfo C = System.Globalization.CultureInfo.InvariantCulture;

            if (MinDate == DateTime.Today)
                return;

            WebClient Client = new WebClient();
            string line;
            string[] columns;

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.Dividends))
            // add a day to mindate since we already have data for mindate
            using (Stream s = Client.OpenRead(MainQueries.GetCSVAddress(Ticker, MinDate.AddDays(1), DateTime.Now, Constants.Dividends)))
            using (StreamReader sr = new StreamReader(s))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                sr.ReadLine();  // first row is header
                line = sr.ReadLine();

                while (line != null)
                {
                    columns = line.Split(',');
                    // columns[0] = Date
                    // columns[1] = Dividend
                    newRecord.SetDateTime((int)MainQueries.Tables.eDividends.Date, Convert.ToDateTime(columns[0], C));
                    newRecord.SetString((int)MainQueries.Tables.eDividends.Ticker, Ticker);
                    newRecord.SetDecimal((int)MainQueries.Tables.eDividends.Amount, Convert.ToDecimal(columns[1], C));

                    rs.Insert(newRecord);

                    line = sr.ReadLine();
                }
            }
        }

        /************************* Correct any missing prices ***********************************/

        private List<Constants.MissingPriceInfo> GetMissingPrices()
        {
            List<Constants.MissingPriceInfo> Tickers = new List<Constants.MissingPriceInfo>();

            using (SqlCeResultSet rs = SQL.ExecuteResultSet(MainQueries.GetMissingPrices()))
                foreach (SqlCeUpdatableRecord rec in rs)
                {
                    Constants.MissingPriceInfo m = new Constants.MissingPriceInfo();
                    m.Ticker = rec.GetString((int)MainQueries.eGetMissingPrices.Ticker);
                    m.Date = rec.GetDateTime((int)MainQueries.eGetMissingPrices.Date);
                    m.PreviousClose = Convert.ToDouble(SQL.ExecuteScalar(MainQueries.GetPreviousTickerClose(m.Ticker, m.Date)));
                    Tickers.Add(m);
                }

            return Tickers;
        }

        private void InsertMissingPrices()
        {
            List<Constants.MissingPriceInfo> Tickers = GetMissingPrices();

            if (Tickers.Count == 0)
                return;

            using (SqlCeResultSet rs = SQL.ExecuteTableUpdate(MainQueries.Tables.ClosingPrices))
            {
                SqlCeUpdatableRecord newRecord = rs.CreateRecord();
                foreach (Constants.MissingPriceInfo m in Tickers)
                {
                    newRecord.SetDateTime((int)MainQueries.Tables.eClosingPrices.Date, m.Date);
                    newRecord.SetString((int)MainQueries.Tables.eClosingPrices.Ticker, m.Ticker);
                    newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Price, (decimal)m.PreviousClose);
                    newRecord.SetDecimal((int)MainQueries.Tables.eClosingPrices.Change, 0);
                    rs.Insert(newRecord);
                }
            }
        }
    }
}