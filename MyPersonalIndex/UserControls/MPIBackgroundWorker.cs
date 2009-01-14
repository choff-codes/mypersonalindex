using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace MyPersonalIndex
{
    class MPIBackgroundWorker : System.ComponentModel.BackgroundWorker 
    {
        public enum MPIUpdateType { UpdatePrices, NAV, NewTicker };
        public MPIUpdateType UpdateType
        { 
            get { return _UpdateType; } 
        }

        public string PortfolioName
        {
            get { return _PortfolioName; }
            set
            {
                if (value.Length > 50)
                    _PortfolioName = value.Substring(0, 47) + "...";
                else
                    _PortfolioName = value;
            }
        }

        public DateTime StartDate
        {
            get { return _StartDate; }
        }

        public int PortfolioID
        {
            get { return _PortfolioID; }
        }

        private MPIUpdateType _UpdateType;
        private string _PortfolioName;
        private DateTime _StartDate;
        private int _PortfolioID;

        public void RunWorkerAsync(MPIUpdateType u)
        {
            _UpdateType = u;
            base.RunWorkerAsync();
        }

        public void RunWorkerAsync(MPIUpdateType u, DateTime StartDate, int PortfolioID)
        {
            _UpdateType = u;
            _StartDate = StartDate;
            _PortfolioID = PortfolioID;
            base.RunWorkerAsync();
        }
    }
}
