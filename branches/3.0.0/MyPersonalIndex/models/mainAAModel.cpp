#include "mainAAModel.h"

//enum { row_Description, row_Value, row_ValueP, row_Target, row_Offset, row_Holdings };
const QStringList aaRow::columns = QStringList() << "Asset Allocation" << "Total Value" << "% of Portfolio" << "Target" << "Offset" << "# Holdings";

const QVariantList aaRow::columnsType = QVariantList() << QVariant(QVariant::String) << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Int);
