#include "frmPriceImport.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDate>
#include "frmPriceImport_UI.h"
#include "tradeDateCalendar.h"

frmPriceImport::frmPriceImport(const historicalPricesMap &pricesMap_, QWidget *parent_):
    QDialog(parent_),
    ui(new frmPriceImport_UI()),
    m_pricesMap(pricesMap_)
{
    ui->setupUI(this);
    connect(ui->importCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->importCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->columnOrderMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui->columnOrderMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(ui->fileBrowseCmd, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->delimiterOther, SIGNAL(toggled(bool)), ui->delimiterOtherTxt, SLOT(setEnabled(bool)));
}

frmPriceImport::~frmPriceImport()
{
    delete ui;
}

void frmPriceImport::moveDown()
{
    if (ui->columnOrder->currentRow() == -1 || ui->columnOrder->currentRow() == ui->columnOrder->count() - 1)
        return;

    int index = ui->columnOrder->currentRow();
    ui->columnOrder->insertItem(index + 1, ui->columnOrder->takeItem(index));
    ui->columnOrder->setCurrentRow(index + 1);
}

void frmPriceImport::moveUp()
{
    if (ui->columnOrder->currentRow() <= 0)
        return;

    int index = ui->columnOrder->currentRow();
    ui->columnOrder->insertItem(index - 1, ui->columnOrder->takeItem(index));
    ui->columnOrder->setCurrentRow(index - 1);
}

void frmPriceImport::browse()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open file...", QString(), "Comma Separated File (*.csv);;Text File (*.txt);;All Files (*)");
    if (!filePath.isEmpty())
        ui->fileTxt->setText(filePath);
}

void frmPriceImport::accept()
{
    if (!validateFilePath())
        return;

    QString delimiter = selectedDelimiter();
    if (delimiter.isEmpty())
        return;

    QFile file(ui->fileTxt->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString dateFormat = selectedDateFormat();
    historicalPricesMap newPricesMap;
    QTextStream in(&file);
    bool header = ui->columnOrderHeaderChk->isChecked();
    QHash<int, int> columns = selectedColumnOrder();

    int row = -1;
    while (!in.atEnd()) {
        ++row;
        QStringList line = in.readLine().split(delimiter);

        if (row == 0 && header)
            continue;

        if (!validateRow(row, line, columns, dateFormat))
            return;

        QString symbol = line.at(columns.value(frmPriceImport_UI::column_Symbol));
        QDate date = QDate::fromString(line.at(columns.value(frmPriceImport_UI::column_Date)), dateFormat);
        double value = line.at(columns.value(frmPriceImport_UI::column_Value)).toDouble();

        QString type = line.at(columns.value(frmPriceImport_UI::column_Type));
        historicalPrices::type priceType =
            type == "Price" ?
                historicalPrices::type_price :
                type == "Dividend" ?
                    historicalPrices::type_dividend :
                    historicalPrices::type_split;

        newPricesMap.getHistoricalPrice(symbol).insert(date.toJulianDay(), value, priceType);
    }

    if (row == -1 || (row == 0 && header)) // nothing imported
    {
        QDialog::reject();
        return;
    }

    if (!mergePrices(newPricesMap, true))
        return;

    mergePrices(newPricesMap, false);
    QDialog::accept();
}

bool frmPriceImport::validateFilePath()
{
    if (ui->fileTxt->text().isEmpty())
    {
        QMessageBox::critical(this, "Import", "No file was chosen to be imported!");
        return false;
    }

    QFile file(ui->fileTxt->text());

    if (!file.exists())
    {
        QMessageBox::critical(this, "Import", "The file path does not exist!");
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Import", "Cannot open the file for reading!");
        return false;
    }

    return true;
}

bool frmPriceImport::validateRow(int row_, const QStringList line_, const QHash<int, int> &columns_, const QString &dateFormat_)
{
    ++row_; // for display
    if (line_.count() != columns_.count())
    {
        QMessageBox::critical(this, "Import", "Incorrect number of columns on row " + QString::number(row_) + ".");
        return false;
    }

    if (line_.at(columns_.value(frmPriceImport_UI::column_Symbol)).isEmpty())
    {
        QMessageBox::critical(this, "Import", "No symbol specified on row " + QString::number(row_) + ".");
        return false;
    }

    QDate date = QDate::fromString(line_.at(columns_.value(frmPriceImport_UI::column_Date)), dateFormat_);
    if (!date.isValid())
    {
        QMessageBox::critical(this, "Import", "Incorrect date format on row " + QString::number(row_) +
            ". Date is \"" + line_.at(columns_.value(frmPriceImport_UI::column_Date)) + "\".");
        return false;
    }
    if (!tradeDateCalendar::isTradeDate(date.toJulianDay()))
    {
        QMessageBox::critical(this, "Import", "Row " + QString::number(row_) +
            " is not a valid US trade date. Date is \"" + line_.at(columns_.value(frmPriceImport_UI::column_Date)) + "\".");
        return false;
    }

    QString type = line_.at(columns_.value(frmPriceImport_UI::column_Type));
    if (type != "Price" && type != "Dividend" && type != "Split")
    {
        QMessageBox::critical(this, "Import", "Incorrect price type on row " + QString::number(row_) +
            ". Price type is \"" + line_.at(columns_.value(frmPriceImport_UI::column_Type)) + "\", but valid choices" +
            " are only Price, Dividend, or Split.");
        return false;
    }

    bool ok;
    line_.at(columns_.value(frmPriceImport_UI::column_Value)).toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Import", "Incorrect value on row " + QString::number(row_) +
            ". Value is \"" + line_.at(columns_.value(frmPriceImport_UI::column_Value)) + "\", but could not be" +
            " converted to a number.");
        return false;
    }

    return true;
}

QString frmPriceImport::selectedDelimiter()
{
    QString delimiter =
            ui->delimiterComma->isChecked() ?
                "," :
                ui->delimiterTab->isChecked() ?
                    "\t" :
                    ui->delimiterPipe->isChecked() ?
                        "|" :
                        ui->delimiterOtherTxt->text();

    if (delimiter.isEmpty())
        QMessageBox::critical(this, "Import", "Specify a delimiter!");

    return delimiter;
}

QString frmPriceImport::selectedDateFormat()
{
    return ui->dateMMDDYYYY->isChecked() ?
                ui->dateMMDDYYYY->text() :
                ui->dateMDYYYY->isChecked() ?
                    ui->dateMDYYYY->text() :
                    ui->dateDMYYYY->isChecked() ?
                        ui->dateDMYYYY->text() :
                        ui->dateYYYYMMDD->text();
}

QHash<int, int> frmPriceImport::selectedColumnOrder()
{
    QHash<int, int> columns;
    for (int i = 0; i < ui->columnOrder->count(); ++i)
        columns.insert(ui->columnOrder->item(i)->data(Qt::UserRole).toInt(), i);
    return columns;
}

bool frmPriceImport::mergePrices(const historicalPricesMap &newPriceMap_, bool dryRun_)
{
    foreach(const historicalPrices &newPrices, newPriceMap_.getHistoricalPrices())
    {
        QMap<int, double> prices = newPrices.values(historicalPrices::type_price);
        QMap<int, double> dividends = newPrices.values(historicalPrices::type_dividend);
        QMap<int, double> splits = newPrices.values(historicalPrices::type_split);

        if (dryRun_)
        {
            // check for duplicates
            bool askAgain = true;
            if (!duplicatePriceCheck(newPrices.symbol(), historicalPrices::type_price, prices, askAgain))
                return false;
            if (!askAgain)
                return true;
            if (!duplicatePriceCheck(newPrices.symbol(), historicalPrices::type_dividend, dividends, askAgain))
                return false;
            if (!askAgain)
                return true;
            if (!duplicatePriceCheck(newPrices.symbol(), historicalPrices::type_split, splits, askAgain))
                return false;
            if (!askAgain)
                return true;

            continue;
        }

        // insert prices
        for(QMap<int, double>::const_iterator i = prices.constBegin(); i != prices.constEnd(); ++i)
            m_pricesMap.getHistoricalPrice(newPrices.symbol()).insert(i.key(), i.value(), historicalPrices::type_price);
        for(QMap<int, double>::const_iterator i = dividends.constBegin(); i != dividends.constEnd(); ++i)
            m_pricesMap.getHistoricalPrice(newPrices.symbol()).insert(i.key(), i.value(), historicalPrices::type_dividend);
        for(QMap<int, double>::const_iterator i = splits.constBegin(); i != splits.constEnd(); ++i)
            m_pricesMap.getHistoricalPrice(newPrices.symbol()).insert(i.key(), i.value(), historicalPrices::type_split);
    }

    return true;
}

bool frmPriceImport::duplicatePriceCheck(const QString &symbol_, historicalPrices::type type_, const QMap<int, double> &values_, bool &askAgain)
{
    for(QMap<int, double>::const_iterator i = values_.constBegin(); i != values_.constEnd(); ++i)
        if (m_pricesMap.getHistoricalPrice(symbol_).contains(i.key(), type_))
        {
            QMessageBox msgBox(this);
            QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
            QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
            msgBox.setDefaultButton(cancelButton);
            msgBox.addButton("OK, don't ask again", QMessageBox::ActionRole);

            msgBox.setWindowTitle("Import");
            QString type =
                type_ == historicalPrices::type_price ?
                          "price" :
                          type_ == historicalPrices::type_dividend ?
                              "dividend" :
                              "split";
            msgBox.setText("Duplicate detected for " + symbol_ + ". Do you want to replace the " + type +
                " on " + QDate::fromJulianDay(i.key()).toString(Qt::SystemLocaleShortDate) + " from " +
                QString::number(m_pricesMap.getHistoricalPrice(symbol_).value(i.key(), type_)) + " to " +
                QString::number(i.value()) + "?");

            msgBox.exec();
            if (msgBox.clickedButton() == okButton)
                continue;
            if (msgBox.clickedButton() == cancelButton)
                return false;
            // Ok, don't ask again
            askAgain = false;
            return true;
        }
    return true;
}
