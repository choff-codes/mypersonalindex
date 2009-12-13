#include "mainAcctModel.h"

//enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_TaxRate, row_TaxLiability, row_Net, row_Holdings };
const QStringList acctRow::columns = QStringList() << "Account" << "Cost Basis" << "Total Value" << "% of Portfolio" << "Gain/Loss" << "% Gain/Loss"
    << "Tax Rate" << "Tax Liability" << "Net Value" << "# of Holdings";

const QVariantList acctRow::columnsType = QVariantList() << QVariant(QVariant::String) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Int);
