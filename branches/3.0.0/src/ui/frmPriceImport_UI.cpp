#include "frmPriceImport_UI.h"
#include <QDialog>

void frmPriceImport_UI::setupUI(QDialog *dialog_)
{
    dialog_->setWindowFlags((dialog_->windowFlags() & (~Qt::WindowContextHelpButtonHint)) | Qt::WindowMinMaxButtonsHint);
    dialog_->setWindowTitle("Import Prices");

    layout = new QGridLayout(dialog_);

    groupBoxDelimiter = new QGroupBox("Delimiter", dialog_);
    delimiterLayout = new QVBoxLayout(groupBoxDelimiter);
    delimiterTab = new QRadioButton("Tab", groupBoxDelimiter);
    delimiterTab->setChecked(true);
    delimiterComma = new QRadioButton("Comma", groupBoxDelimiter);
    delimiterPipe = new QRadioButton("Pipe", groupBoxDelimiter);

    delimiterOtherLayout = new QHBoxLayout();
    delimiterOther = new QRadioButton("Other:", groupBoxDelimiter);
    delimiterOtherTxt = new QLineEdit(groupBoxDelimiter);
    delimiterOtherTxt->setMaxLength(3);
    delimiterOtherTxt->setEnabled(false);
    delimiterOtherTxt->setMaximumWidth(delimiterOtherTxt->fontMetrics().maxWidth() * 3);
    delimiterOtherLayout->addWidget(delimiterOther);
    delimiterOtherLayout->addWidget(delimiterOtherTxt);

    delimiterLayout->addWidget(delimiterTab);
    delimiterLayout->addWidget(delimiterComma);
    delimiterLayout->addWidget(delimiterPipe);
    delimiterLayout->addLayout(delimiterOtherLayout);

    layout->addWidget(groupBoxDelimiter, 0, 0, 1, 1);

    groupBoxDate = new QGroupBox("Date Format", dialog_);
    dateLayout = new QVBoxLayout(groupBoxDate);
    dateMMDDYYYY = new QRadioButton("MM/DD/YYYY", groupBoxDate);
    dateMMDDYYYY->setChecked(true);
    dateYYYYMMDD = new QRadioButton("YYYY-MM-DD", groupBoxDate);
    dateDDMMYYYY = new QRadioButton("DD/MM/YYYY", groupBoxDate);
    dateDDMMMYYYY = new QRadioButton("DD-MMM-YYYY", groupBoxDate);

    dateLayout->addWidget(dateMMDDYYYY);
    dateLayout->addWidget(dateYYYYMMDD);
    dateLayout->addWidget(dateDDMMYYYY);
    dateLayout->addWidget(dateDDMMMYYYY);

    layout->addWidget(groupBoxDate, 0, 1, 1, 1);

    groupBoxColumnOrder = new QGroupBox("Column Order", dialog_);
    columnOrderLayout = new QGridLayout(groupBoxColumnOrder);
    columnOrder = new QListWidget(groupBoxColumnOrder);
    columnOrder->setMaximumHeight(columnOrder->fontMetrics().lineSpacing() * 8);
    QListWidgetItem *column1 = new QListWidgetItem("Symbol", columnOrder);
    column1->setData(Qt::UserRole, column_Symbol);
    QListWidgetItem *column2 = new QListWidgetItem("Date", columnOrder);
    column2->setData(Qt::UserRole, column_Date);
    QListWidgetItem *column3 = new QListWidgetItem("Price Type", columnOrder);
    column3->setData(Qt::UserRole, column_Type);
    QListWidgetItem *column4 = new QListWidgetItem("Value", columnOrder);
    column4->setData(Qt::UserRole, column_Value);
    columnOrder->addItem(column1);
    columnOrder->addItem(column2);
    columnOrder->addItem(column3);
    columnOrder->addItem(column4);

    columnOrderMoveLayout = new QVBoxLayout();
    columnOrderMoveUp = new QToolButton(groupBoxColumnOrder);
    columnOrderMoveUp->setArrowType(Qt::UpArrow);
    columnOrderMoveDown = new QToolButton(groupBoxColumnOrder);
    columnOrderMoveDown->setArrowType(Qt::DownArrow);

    columnOrderMoveLayout->addWidget(columnOrderMoveUp);
    columnOrderMoveLayout->addWidget(columnOrderMoveDown);

    columnOrderHeaderChk = new QCheckBox("Header Row", groupBoxColumnOrder);

    columnOrderLayout->addWidget(columnOrder, 0, 0, 1, 1);
    columnOrderLayout->addLayout(columnOrderMoveLayout, 0, 1, 1, 1);
    columnOrderLayout->addWidget(columnOrderHeaderChk, 1, 0, 1, 2);

    layout->addWidget(groupBoxColumnOrder, 0, 2, 1, 1);

    description = new QLabel(dialog_);
    description->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    description->setWordWrap(true);
    description->setText("Columns should contain the following values:\n\nSymbol: The symbol of the security\n\nDate: The date in the format selected\n\nPrice Type: \"Price\", \"Dividend\", or \"Split\"\n\nValue: The price, dividend, or split for the given date and price type.  The price and dividend should be a valid amount.  If it is a split, then it should be the number of shares distributed.  A 2:1 split would be 2 and a 3:2 split would be 1.5.\n\nSample:\nVTI,2011-01-03,Price,100.0\nVTI,2011-01-04,Price,101.2\nVTI,2011-06-30,Dividend,0.60\nVTI,2008-05-30,Split,1.5\n");

    layout->addWidget(description, 1, 0, 1, 3);

    groupBoxFile = new QGroupBox("File", dialog_);
    fileLayout = new QHBoxLayout(groupBoxFile);
    fileTxt = new QLineEdit(groupBoxFile);
    fileBrowseCmd = new QPushButton("Browse...", groupBoxFile);

    fileLayout->addWidget(fileTxt);
    fileLayout->addWidget(fileBrowseCmd);

    layout->addWidget(groupBoxFile, 2, 0, 1, 3);

    importCancelBtn = new QDialogButtonBox(dialog_);
    importCancelBtn->setStandardButtons(QDialogButtonBox::Cancel);
    importCancelBtn->addButton("Import", QDialogButtonBox::AcceptRole);

    layout->addWidget(importCancelBtn, 3, 2, 1, 1);
}
