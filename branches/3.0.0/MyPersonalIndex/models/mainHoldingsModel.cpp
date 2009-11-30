#include "mainHoldingsModel.h"

//enum { row_Active, row_Ticker, row_Cash, row_Price, row_Shares, row_Avg, row_Cost, row_Value, row_ValueP, row_Gain, row_GainP, row_Acct, row_ID };
const QStringList holdingsRow::columns = QStringList() << "Active" << "Symbol" << "Cash" << "Closing Price" << "Shares" << "Avg Price\nPer Share"
     << "Cost Basis" << "Total Value" << "% of\nPortfolio" << "Gain/Loss" << "% Gain/Loss" << "Account";

const QVariantList holdingsRow::columnsType = QVariantList() << QVariant(QVariant::Int) << QVariant(QVariant::String) << QVariant(QVariant::Int)
     << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::String);
