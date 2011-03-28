CREATE TABLE "HistoricalPrice" ("Date" INTEGER,"Symbol" VARCHAR,"Type" INTEGER,"Value" NUMERIC);
CREATE TABLE "Portfolio" ("ID" INTEGER,"Description" VARCHAR,"StartDate" INTEGER);
CREATE TABLE "PortfolioAA" ("ID" INTEGER, "PortfolioID" INTEGER, "Description" VARCHAR, "Target" NUMERIC, "Threshold" INTEGER, "RebalanceBand" NUMERIC, "Hide" INTEGER);
CREATE TABLE "PortfolioAccount" ("ID" INTEGER, "PortfolioID" INTEGER, "Description" VARCHAR, "TaxRate" NUMERIC, "TaxDeferred" INTEGER, "CostBasis" INTEGER, "Hide" INTEGER);
CREATE TABLE "PortfolioSecurity" ("ID","PortfolioID" INTEGER,"Symbol" VARCHAR,"Account" INTEGER,"Expense" DOUBLE,"DivReinvest" INTEGER,"CashAccount" INTEGER,"IncludeInCalc" INTEGER,"Hide" INTEGER, "Note" VARCHAR, "Dividends" INTEGER);
CREATE TABLE "PortfolioSecurityAA" ("SecurityID" INTEGER, "AAID" INTEGER, "Percent" NUMERIC);
CREATE TABLE "PortfolioSecurityTrade" ("ID" INTEGER PRIMARY KEY  NOT NULL , "SecurityID" INTEGER, "Type" INTEGER, "Value" NUMERIC, "Price" NUMERIC, "Commission" NUMERIC, "CashAccountID" INTEGER, "Frequency" INTEGER, "Date" INTEGER, "StartDate" INTEGER, "EndDate" INTEGER, "Description" VARCHAR, "PriceType" INTEGER);
CREATE TABLE "PortfolioSecurityTradeExecution" ("SecurityID" INTEGER,"Date" INTEGER,"Shares" NUMERIC,"Price" NUMERIC,"Commission" NUMERIC, "AssociatedTradeID" INTEGER);
CREATE VIEW "PortfolioSecurityAAView" AS SELECT aa.*, s.PortfolioID FROM PortfolioSecurityAA aa INNER JOIN PortfolioSecurity s ON aa.SecurityID = s.ID;
CREATE VIEW "PortfolioSecurityTradeExecutionView" AS SELECT e.*, s.PortfolioID FROM PortfolioSecurityTradeExecution e INNER JOIN PortfolioSecurity s ON e.SecurityID = s.ID;
CREATE VIEW "PortfolioSecurityTradeView" AS SELECT trades.*, s.PortfolioID FROM PortfolioSecurityTrade trades INNER JOIN PortfolioSecurity s ON trades.SecurityID = s.ID;
CREATE TRIGGER "PortfolioSecurityTrigger" BEFORE DELETE ON PortfolioSecurity FOR EACH ROW BEGIN
DELETE FROM PortfolioSecurityAA WHERE SecurityID = old.ID;
DELETE FROM PortfolioSecurityTrade WHERE SecurityID = old.ID;
DELETE FROM PortfolioSecurityTradeExecution WHERE SecurityID = old.ID; END;
CREATE TRIGGER "PortfolioTrigger" BEFORE DELETE ON Portfolio FOR EACH ROW BEGIN
DELETE FROM PortfolioSecurity WHERE PortfolioID = old.ID;
DELETE FROM PortfolioAA WHERE PortfolioID = old.ID;
DELETE FROM PortfolioAccount WHERE PortfolioID = old.ID; END;
