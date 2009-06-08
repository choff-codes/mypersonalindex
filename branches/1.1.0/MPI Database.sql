-- 4/26/2009 1:05 AM
-- Database information:
-- locale identifier: 1033
-- encryption mode: 
-- case sensitive: False

CREATE TABLE [AA] ([ID] int NOT NULL  IDENTITY (1,1) 
, [Portfolio] int NULL   
, [AA] nvarchar(255) NULL  
, [Target] numeric(6,2) NULL  
);
GO
CREATE TABLE [Accounts] ([ID] int NOT NULL  IDENTITY (1,1) 
, [Portfolio] int NULL   
, [Name] nvarchar(255) NULL  
, [TaxRate] numeric(6,2) NULL  
);
GO
CREATE TABLE [AvgPricePerShare] ([Ticker] int NULL   
, [Price] numeric(12,4) NULL  
);
GO
CREATE TABLE [ClosingPrices] ([Date] datetime NULL   
, [Ticker] nvarchar(10) NULL  
, [Price] numeric(12,4) NULL  
, [Change] numeric(18,4) NULL  
);
GO
CREATE TABLE [CustomTrades] ([TickerID] int NULL   
, [Portfolio] int NULL   
, [TradeType] int NULL   
, [Frequency] int NULL   
, [Dates] nvarchar(4000) NULL  
, [Value] numeric(18,4) NULL  
);
GO
CREATE TABLE [Dividends] ([Date] datetime NULL   
, [Ticker] nvarchar(10) NULL  
, [Amount] numeric(12,4) NULL  
);
GO
CREATE TABLE [NAV] ([Portfolio] int NULL   
, [Date] datetime NULL   
, [TotalValue] numeric(24,4) NULL  
, [NAV] numeric(24,8) NULL  
, [Change] numeric(18,4) NULL  
);
GO
CREATE TABLE [Portfolios] ([ID] int NOT NULL  IDENTITY (1,1) 
, [Name] nvarchar(100) NULL  
, [Dividends] bit NULL   
, [CostCalc] int NULL   
, [HoldingsShowHidden] bit NULL   
, [HoldingsSort] nvarchar(250) NULL  
, [NAVSort] bit NULL   
, [NAVStartValue] numeric(12,2) NULL  
, [AAThreshold] int NULL   
, [AASort] nvarchar(250) NULL  
, [AAShowBlank] bit NULL   
, [AcctSort] nvarchar(250) NULL  
, [AcctShowBlank] bit NULL   
, [CorrelationShowHidden] bit NULL   
, [StartDate] datetime NULL   
);
GO
CREATE TABLE [Settings] ([DataStartDate] datetime NULL   
, [LastPortfolio] int NULL   
, [Version] numeric(6,2) NULL  
, [WindowX] int NULL   
, [WindowY] int NULL   
, [WindowHeight] int NULL   
, [WindowWidth] int NULL   
, [WindowState] int NULL   
, [Splits] bit NULL   
, [TickerDiv] bit NULL   
);
GO
CREATE TABLE [Splits] ([Date] datetime NULL   
, [Ticker] nvarchar(10) NULL  
, [Ratio] numeric(10,4) NULL  
);
GO
CREATE TABLE [Stats] ([Portfolio] int NULL   
, [Statistic] int NULL   
, [Location] int NULL   
);
GO
CREATE TABLE [Tickers] ([ID] int NOT NULL  IDENTITY (1,1) 
, [Ticker] nvarchar(10) NULL  
, [Portfolio] int NULL   
, [Active] bit NULL   
, [AA] int NULL   
, [Acct] int NULL   
, [Hide] bit NULL   
);
GO
CREATE TABLE [Trades] ([Date] datetime NULL   
, [Portfolio] int NULL   
, [TickerID] int NULL   
, [Ticker] nvarchar(10) NULL  
, [Shares] numeric(12,4) NULL  
, [Price] numeric(12,4) NULL  
, [ID] int NULL   
, [Custom] bit NULL   
);
GO
CREATE TABLE [UserStatistics] ([ID] int NOT NULL  IDENTITY (1,1) 
, [SQL] nvarchar(4000) NULL  
, [Description] nvarchar(100) NULL  
, [Format] int NULL   
);
GO
SET IDENTITY_INSERT [AA] ON
GO
SET IDENTITY_INSERT [AA] OFF
GO
SET IDENTITY_INSERT [Accounts] ON
GO
SET IDENTITY_INSERT [Accounts] OFF
GO
SET IDENTITY_INSERT [Portfolios] ON
GO
SET IDENTITY_INSERT [Portfolios] OFF
GO
Insert Into [Settings] ([DataStartDate],[LastPortfolio],[Version],[WindowX],[WindowY],[WindowHeight],[WindowWidth],[WindowState],[Splits],[TickerDiv]) Values ({ts '2008-01-02 12:00:00'},null,2.00,null,null,null,null,null,1,1);
GO
SET IDENTITY_INSERT [Tickers] ON
GO
SET IDENTITY_INSERT [Tickers] OFF
GO
SET IDENTITY_INSERT [UserStatistics] ON
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (1,N'SELECT NAV AS NAV
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date = ''%PreviousDay%''',N'Beginning Index Value',2);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (2,N'SELECT TotalValue
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date = ''%PreviousDay%''',N'Beginning Value',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (3,N'SELECT SUM(c.Price * b.Shares) AS CostBasis
FROM Tickers AS a 
LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares 
            FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares 
                    FROM Trades a 
                    LEFT JOIN Splits b 
                        ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' 
                    WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' 
                    GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades 
            GROUP BY TickerID) AS b 
    ON a.ID = b.TickerID 
LEFT JOIN (SELECT Ticker, Price 
            FROM AvgPricePerShare) AS c 
    ON a.ID = c.Ticker 
WHERE a.Active = 1 AND Portfolio = %Portfolio%',N'Cost Basis',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (4,N'SELECT NAV AS NAV
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date = ''%EndDate%''',N'Current Index Value',2);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (5,N'SELECT %TotalValue%',N'Current Value',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (6,N'SELECT (CASE WHEN Days = 0 THEN 0
ELSE 100 * POWER(NAV, 1.0 / Days) - 100 END) AS DailyReturn
FROM
	(SELECT a.NAV / b.NAV AS NAV
	FROM NAV a
	CROSS JOIN 
	  (SELECT NAV FROM NAV
	   WHERE Portfolio = %Portfolio%
       AND Date = ''%PreviousDay%'') b
	WHERE Portfolio = %Portfolio%
	AND Date = ''%EndDate%'') a
CROSS JOIN
	(SELECT COUNT(*) AS Days
	FROM NAV
	WHERE Portfolio = %Portfolio%
	AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%'') b',N'Daily Return',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (7,N'SELECT SQRT((SUM(POWER(Change,2))-POWER(SUM(Change)/COUNT(*),2)) / COUNT(*))
FROM NAV 
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Daily Standard Deviation',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (8,N'SELECT MAX(Date)
FROM NAV
WHERE Portfolio = %Portfolio% 
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Date',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (9,N'SELECT COUNT(*) 
FROM NAV 
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Days Invested',3);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (10,N'SELECT SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(1 - (e.TaxRate/100), 1.0) ELSE 1.0 END)) AS GainLoss
FROM Tickers AS a 
LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares 
            FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares 
                    FROM Trades a 
                    LEFT JOIN Splits b 
                        ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' 
                    WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' 
                    GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades 
            GROUP BY TickerID) AS b 
    ON a.ID = b.TickerID 
 LEFT JOIN (SELECT Ticker, Price 
            FROM AvgPricePerShare) AS c 
    ON a.ID = c.Ticker 
LEFT JOIN (SELECT Ticker, Price 
            FROM ClosingPrices 
            WHERE DATE = ''%EndDate%'') AS d 
    ON a.Ticker = d.Ticker 
LEFT JOIN (SELECT ID, Name, TaxRate 
            FROM Accounts 
            WHERE Portfolio = %Portfolio%) AS e 
    ON a.Acct = e.ID 
WHERE a.Active = 1 AND Portfolio = %Portfolio%',N'Gain/Loss',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (11,N'SELECT (CASE WHEN Days = 0 THEN 0
ELSE 100 * POWER(NAV, 1.0 / (Days * 6.5)) - 100 END) AS DailyReturn
FROM
	(SELECT a.NAV / b.NAV AS NAV
	FROM NAV a
	CROSS JOIN 
	  (SELECT NAV FROM NAV
	   WHERE Portfolio = %Portfolio%
       AND Date = ''%PreviousDay%'') b
	WHERE Portfolio = %Portfolio%
	AND Date = ''%EndDate%'') a
CROSS JOIN
	(SELECT COUNT(*) AS Days
	FROM NAV
	WHERE Portfolio = %Portfolio%
	AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%'') b',N'Hourly Return',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (12,N'SELECT MIN(Change)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Max % Down',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (13,N'SELECT Date
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''
AND Change IN
(SELECT MIN(Change)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%'')',N'Max % Down Day',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (14,N'SELECT MAX(Change)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Max % Up',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (15,N'SELECT Date
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''
AND Change IN
(SELECT MAX(Change)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%'')',N'Max % Up Day',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (16,N'SELECT MAX(NAV)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''',N'Maximum Index Value',2);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (17,N'SELECT Date
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
AND NAV IN (
	SELECT MAX(NAV)
	FROM NAV
	WHERE Portfolio = %Portfolio%
	AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
)',N'Maximum Index Value Day',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (18,N'SELECT MAX(TotalValue)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''',N'Maximum Portfolio Value',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (19,N'SELECT Date
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
AND TotalValue IN (
	SELECT MAX(TotalValue)
	FROM NAV
	WHERE Portfolio = %Portfolio%
	AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
)',N'Maximum Portfolio Value Day',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (20,N'SELECT MIN(NAV)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''',N'Minimum Index Value',2);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (21,N'SELECT Date
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
AND NAV IN (
	SELECT MIN(NAV)
	FROM NAV
	WHERE Portfolio = %Portfolio%
	AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
)',N'Minimum Index Value Day',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (22,N'SELECT MIN(TotalValue)
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''',N'Minimum Portfolio Value',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (23,N'SELECT Date
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
AND TotalValue IN (
	SELECT MIN(TotalValue)
	FROM NAV
	WHERE Portfolio = %Portfolio%
	AND Date BETWEEN ''%PreviousDay%'' AND ''%EndDate%''
)',N'Minimum Portfolio Value Day',4);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (24,N'SELECT (CASE WHEN DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 30 = 0 THEN a.NAV / b.NAV - 1
              ELSE POWER(a.NAV / b.NAV, 1.0 / (DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 30)) - 1 END) * 100 AS MonthlyReturn
FROM NAV a
CROSS JOIN 
	  (SELECT NAV FROM NAV
	   WHERE Portfolio = %Portfolio%
       AND Date = ''%PreviousDay%'') b
WHERE Portfolio = %Portfolio%
AND Date = ''%EndDate%''',N'Monthly Return',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (25,N'SELECT SQRT(22) * SQRT((SUM(POWER(Change,2))-POWER(SUM(Change)/COUNT(*),2)) / COUNT(*))
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Monthly Standard Deviation',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (26,N'SELECT ''%TotalValue%'' - TotalValue
FROM NAV a
WHERE Portfolio = %Portfolio%
AND Date = ''%PreviousDay%''',N'Net Change',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (27,N'SELECT (a.NAV / b.NAV - 1) * 100 AS NAV
FROM NAV a
CROSS JOIN (SELECT NAV
			FROM NAV
			WHERE Portfolio = %Portfolio%
			AND Date = ''%PreviousDay%'') b
WHERE Portfolio = %Portfolio%
AND Date = ''%EndDate%''',N'Overall Return',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (28,N'SELECT (CASE WHEN z >= 0 THEN  (1.0 - 0.39894228 * exp( -z * z / 2.0 ) * t * ( t *( t * ( t * ( t * 1.330274429 + -1.821255978 ) + 1.781477937 ) + -0.356563782 ) + 0.319381530 ))
        ELSE ( 0.39894228 * exp( -z * z / 2.0 ) * t * ( t *( t * ( t * ( t * 1.330274429 + -1.821255978 ) + 1.781477937 ) + -0.356563782 ) + 0.319381530 )) END) * 100 AS Probability
FROM (SELECT (CASE WHEN YearlyGain / StdDev >= 0 THEN 1.0 / ( 1.0 + 0.2316419 * (YearlyGain / StdDev) ) ELSE 1.0 / ( 1.0 - 0.2316419 * (YearlyGain / StdDev) ) END) AS t,
	  YearlyGain / StdDev AS z
	  FROM (SELECT SQRT(250) * SQRT((SUM(POWER(Change,2))-POWER(SUM(Change)/COUNT(*),2)) / COUNT(*)) / 100 AS StdDev
			FROM NAV 
			WHERE Portfolio = %Portfolio%
			AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%'') a
	  CROSS JOIN
			(SELECT (CASE WHEN DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 365 = 0 THEN a.NAV / b.NAV - 1
				ELSE POWER(a.NAV / b.NAV, 1.0 / (DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 365)) - 1 END) AS YearlyGain
			FROM NAV a
			CROSS JOIN 
				(SELECT NAV FROM NAV
				WHERE Portfolio = %Portfolio%
				AND Date = ''%PreviousDay%'') b
			WHERE Portfolio = %Portfolio%
			AND Date = ''%EndDate%'') b
) Calcs',N'Probability of Yearly Gain',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (29,N'SELECT 100 - (CASE WHEN z >= 0 THEN  (1.0 - 0.39894228 * exp( -z * z / 2.0 ) * t * ( t *( t * ( t * ( t * 1.330274429 + -1.821255978 ) + 1.781477937 ) + -0.356563782 ) + 0.319381530 ))
        ELSE ( 0.39894228 * exp( -z * z / 2.0 ) * t * ( t *( t * ( t * ( t * 1.330274429 + -1.821255978 ) + 1.781477937 ) + -0.356563782 ) + 0.319381530 )) END) * 100 AS Probability
FROM (SELECT (CASE WHEN YearlyGain / StdDev >= 0 THEN 1.0 / ( 1.0 + 0.2316419 * (YearlyGain / StdDev) ) ELSE 1.0 / ( 1.0 - 0.2316419 * (YearlyGain / StdDev) ) END) AS t,
	  YearlyGain / StdDev AS z
	  FROM (SELECT SQRT(250) * SQRT((SUM(POWER(Change,2))-POWER(SUM(Change)/COUNT(*),2)) / COUNT(*)) / 100 AS StdDev
			FROM NAV 
			WHERE Portfolio = %Portfolio%
			AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%'') a
	  CROSS JOIN
			(SELECT (CASE WHEN DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 365 = 0 THEN a.NAV / b.NAV - 1
				ELSE POWER(a.NAV / b.NAV, 1.0 / (DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 365)) - 1 END) AS YearlyGain
			FROM NAV a
			CROSS JOIN 
				(SELECT NAV FROM NAV
				WHERE Portfolio = %Portfolio%
				AND Date = ''%PreviousDay%'') b
			WHERE Portfolio = %Portfolio%
			AND Date = ''%EndDate%'') b
) Calcs',N'Probability Of Yearly Loss',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (30,N'SELECT SUM(((d.Price - c.Price) * b.Shares) * (CASE WHEN d.Price > c.Price THEN Coalesce(e.TaxRate/100, 0.0) ELSE 0.0 END)) AS TaxLiability
FROM Tickers AS a 
LEFT JOIN (SELECT TickerID, SUM(Shares) AS Shares 
            FROM (SELECT a.TickerID, a.Shares * CAST(COALESCE(EXP(SUM(LOG(b.Ratio))), 1.0) AS DECIMAL(18,4)) as Shares 
                    FROM Trades a 
                    LEFT JOIN Splits b 
                        ON a.Ticker = b.Ticker AND b.Date BETWEEN a.Date AND ''%EndDate%'' 
                    WHERE a.Portfolio = %Portfolio% AND a.Date <= ''%EndDate%'' 
                    GROUP BY a.ID, a.Custom, a.TickerID, a.Shares) AllTrades 
            GROUP BY TickerID) AS b 
    ON a.ID = b.TickerID 
 LEFT JOIN (SELECT Ticker, Price 
            FROM AvgPricePerShare) AS c 
    ON a.ID = c.Ticker 
LEFT JOIN (SELECT Ticker, Price 
            FROM ClosingPrices 
            WHERE DATE = ''%EndDate%'') AS d 
    ON a.Ticker = d.Ticker 
LEFT JOIN (SELECT ID, Name, TaxRate 
            FROM Accounts 
            WHERE Portfolio = %Portfolio%) AS e 
    ON a.Acct = e.ID 
WHERE a.Active = 1 AND Portfolio = %Portfolio%',N'Tax Liabliity',0);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (31,N'SELECT (CASE WHEN DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 365 = 0 THEN a.NAV / b.NAV - 1
              ELSE POWER(a.NAV / b.NAV, 1.0 / (DATEDIFF(day, ''%StartDate%'', ''%EndDate%'') / 365)) - 1 END) * 100 AS YearlyReturn
FROM NAV a
CROSS JOIN 
	  (SELECT NAV FROM NAV
	   WHERE Portfolio = %Portfolio%
       AND Date = ''%PreviousDay%'') b
WHERE Portfolio = %Portfolio%
AND Date = ''%EndDate%''',N'Yearly Return',1);
GO
Insert Into [UserStatistics] ([ID],[SQL],[Description],[Format]) Values (32,N'SELECT SQRT(250) * SQRT((SUM(POWER(Change,2))-POWER(SUM(Change)/COUNT(*),2)) / COUNT(*))
FROM NAV
WHERE Portfolio = %Portfolio%
AND Date BETWEEN ''%StartDate%'' AND ''%EndDate%''',N'Yearly Standard Deviation',1);
GO
SET IDENTITY_INSERT [UserStatistics] OFF
GO
ALTER TABLE [AA] ADD PRIMARY KEY ([ID]);
GO
ALTER TABLE [Accounts] ADD PRIMARY KEY ([ID]);
GO
ALTER TABLE [Portfolios] ADD PRIMARY KEY ([ID]);
GO
ALTER TABLE [Tickers] ADD PRIMARY KEY ([ID]);
GO
ALTER TABLE [UserStatistics] ADD PRIMARY KEY ([ID]);
GO
GO
CREATE UNIQUE INDEX [DateTicker] ON [ClosingPrices] ([Date] Asc,[Ticker] Asc);
GO
CREATE INDEX [TickerPortfolio] ON [CustomTrades] ([TickerID] Asc,[Portfolio] Asc);
GO
CREATE UNIQUE INDEX [DateTicker] ON [Dividends] ([Date] Asc,[Ticker] Asc);
GO
CREATE UNIQUE INDEX [DatePortfolio] ON [NAV] ([Portfolio] Asc,[Date] Asc);
GO
CREATE UNIQUE INDEX [DateTicker] ON [Splits] ([Date] Asc,[Ticker] Asc);
GO
CREATE INDEX [DateTickerPortfolio] ON [Trades] ([Date] Asc,[Portfolio] Asc,[TickerID] Asc);
GO

