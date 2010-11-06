CREATE TABLE "HistoricalPrice" ("Date" INTEGER,"Symbol" VARCHAR,"Type" INTEGER,"Value" NUMERIC);
CREATE TABLE "Portfolio" ("ID" INTEGER PRIMARY KEY  NOT NULL,"Description" VARCHAR,"StartValue" INTEGER,"StartDate" INTEGER);
CREATE TABLE "PortfolioAA" ("ID" INTEGER PRIMARY KEY  NOT NULL , "PortfolioID" INTEGER, "Description" VARCHAR, "Target" NUMERIC, "Threshold" INTEGER, "RebalanceBand" NUMERIC, "Hide" INTEGER);
CREATE TABLE "PortfolioAccount" ("ID" INTEGER PRIMARY KEY  NOT NULL , "PortfolioID" INTEGER, "Description" VARCHAR, "TaxRate" NUMERIC, "TaxDeferred" INTEGER, "CostBasis" INTEGER, "Hide" INTEGER);
CREATE TABLE "PortfolioSecurity" ("ID" INTEGER PRIMARY KEY  NOT NULL ,"PortfolioID" INTEGER,"Symbol" VARCHAR,"Account" INTEGER,"Expense" DOUBLE,"DivReinvest" INTEGER,"CashAccount" INTEGER,"IncludeInCalc" INTEGER,"Hide" INTEGER, "Note" VARCHAR, "Dividends" INTEGER);
CREATE TABLE "PortfolioSecurityAA" ("SecurityID" INTEGER, "AAID" INTEGER, "Percent" NUMERIC);
CREATE TABLE "PortfolioSecurityTrade" ("ID" INTEGER PRIMARY KEY  NOT NULL , "SecurityID" INTEGER, "Type" INTEGER, "Value" NUMERIC, "Price" NUMERIC, "Commission" NUMERIC, "CashAccountID" INTEGER, "Frequency" INTEGER, "Date" INTEGER, "StartDate" INTEGER, "EndDate" INTEGER, "Description" VARCHAR);
CREATE TABLE "PortfolioSecurityTradeExecution" ("SecurityID" INTEGER,"Date" INTEGER,"Shares" NUMERIC,"Price" NUMERIC,"Commission" NUMERIC, "AssociatedTradeID" INTEGER);
CREATE TABLE "Settings" ("Version" INTEGER NOT NULL);
CREATE VIEW "PortfolioSecurityAAView" AS SELECT aa.*, s.PortfolioID FROM PortfolioSecurityAA aa INNER JOIN PortfolioSecurity s ON aa.SecurityID = s.ID;
CREATE VIEW "PortfolioSecurityTradeExecutionView" AS SELECT e.*, s.PortfolioID FROM PortfolioSecurityTradeExecution e INNER JOIN PortfolioSecurity s ON e.SecurityID = s.ID;
CREATE VIEW "PortfolioSecurityTradeView" AS SELECT trades.*, s.PortfolioID FROM PortfolioSecurityTrade trades INNER JOIN PortfolioSecurity s ON trades.SecurityID = s.ID;
CREATE TRIGGER "PortfolioSecurityTrigger" BEFORE DELETE ON PortfolioSecurity FOR EACH ROW BEGIN
DELETE FROM PortfolioSecurityAA WHERE PortfolioSecurityAA.SecurityID = old.ID;
DELETE FROM PortfolioSecurityTrade WHERE PortfolioSecurityTrade.SecurityID = old.ID;
DELETE FROM PortfolioSecurityTradeExecution WHERE PortfolioSecurityTrade.SecurityID = old.ID; END;
CREATE TRIGGER "PortfolioTrigger" BEFORE DELETE ON Portfolio FOR EACH ROW BEGIN
DELETE FROM PortfolioSecurity WHERE PortfolioSecurity.PortfolioID = old.ID;
DELETE FROM PortfolioAA WHERE PortfolioAA.PortfolioID = old.ID;
DELETE FROM PortfolioAcctount WHERE PortfolioAccount.PortfolioID = old.ID; END;
CREATE INDEX "HistoricalPriceIndex" ON "HistoricalPrice" ("Symbol" ASC);
