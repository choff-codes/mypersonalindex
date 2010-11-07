#include "mpiTableView.h"
#include <QTextStream>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include "functions.h"

void mpiTableView::copyTable()
{
    QList<int> rows;
    foreach(const QModelIndex &q, selectionModel()->selectedRows())
        rows.append(q.row());

    QStringList output = formatTableForExport(rows, "\t");

    if (output.isEmpty())
        return;

    QApplication::clipboard()->setText(output.join("\n"));
}

void mpiTableView::exportTable()
{
    QList<int> rows;
    QString fileType, filePath;
    QString delimiter = "\t";

    for(int i = 0; i < model()->rowCount(); ++i)
        rows.append(i);

    filePath = QFileDialog::getSaveFileName(parentWidget(), "Export to...", QDir::homePath(),
        "Tab Delimited File (*.txt);;Comma Delimited File (*.csv);;Pipe Delimited File (*.txt)", &fileType);

    if (filePath.isEmpty())
        return;

    delimiter = fileType.contains("Tab") ? "\t" : fileType.contains("Pipe") ? "|" : ",";

    QStringList output = formatTableForExport(rows, delimiter);
    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
    {
        QMessageBox::critical(parentWidget(), "Error!", "Could not save file, the file path cannot be opened!");
        return;
    }

    QTextStream out(&file);
    out << output.join("\n");
}

QStringList mpiTableView::formatTableForExport(const QList<int> &rows_, const QString &delimiter_)
{
    QStringList textToExport;
    textToExport.append(formatHeaderForExport(delimiter_));

    for(int i = 0; i < rows_.count(); ++i)
    {
        QStringList rowText;

        if (m_hasRowLabels)
            rowText.append(functions::formatForExport(model()->headerData(rows_.at(i), Qt::Vertical, Qt::DisplayRole), delimiter_));

        for (int x = 0; x < model()->columnCount(); ++x)
        {
            QVariant v = model()->data(model()->index(rows_.at(i), x), Qt::CheckStateRole); // is this a true/false column?

            if (v.isNull())
                rowText.append(functions::formatForExport(model()->data(model()->index(rows_.at(i), x), Qt::DisplayRole), delimiter_));
            else
                rowText.append(v.toInt() == Qt::Checked ? "Yes" : "No");
        }

        textToExport.append(rowText.join(delimiter_));
    }

    return textToExport;
}

QString mpiTableView::formatHeaderForExport(const QString &delimiter_)
{
    QStringList header;

    if (m_hasRowLabels) // if vertical row labels, top corner will be blank
        header.append("");

    for(int i = 0; i < model()->columnCount(); ++i)
        header.append(functions::formatForExport(model()->headerData(i, Qt::Horizontal, Qt::DisplayRole), delimiter_));

    return header.join(delimiter_);
}
