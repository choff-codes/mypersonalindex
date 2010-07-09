#include "mpiTableView.h"

void mpiTableView::exportTable(bool copy)
{
    QList<int> rows;
    QString fileType, filePath;
    QString delimiter = "\t";

    if (!copy) // export
    {
        for(int i = 0; i < model()->rowCount(); ++i)
            rows.append(i);

        filePath = QFileDialog::getSaveFileName(parentWidget(), "Export to...", QDir::homePath(),
            "Tab Delimited File (*.txt);;Comma Delimited File (*.csv);;Pipe Delimited File (*.txt)", &fileType);

        if (filePath.isEmpty())
            return;

        delimiter = fileType.contains("Tab") ? "\t" : fileType.contains("Pipe") ? "|" : ",";
    }
    else // copy
    {
        foreach(const QModelIndex &q, selectionModel()->selectedRows())
            rows.append(q.row());

        if (rows.isEmpty())
            return;
    }

    QStringList line, lines;

    if (m_hasRowLabels)
        line.append("");

    for(int i = 0; i < model()->columnCount(); ++i)
        line.append(functions::formatForExport(model()->headerData(i, Qt::Horizontal, Qt::DisplayRole), delimiter));

    lines.append(line.join(delimiter));

    for(int x = 0; x < rows.count(); ++x)
    {
        line.clear();
        if (m_hasRowLabels)
            line.append(functions::formatForExport(model()->headerData(rows.at(x), Qt::Vertical, Qt::DisplayRole), delimiter));
        for (int i = 0; i < model()->columnCount(); ++i)
        {
            QVariant v = model()->data(model()->index(rows.at(x), i), Qt::CheckStateRole);
            if (v.isNull())
                line.append(functions::formatForExport(model()->data(model()->index(rows.at(x), i), Qt::DisplayRole), delimiter));
            else
                line.append(v.toInt() == Qt::Checked ? "Yes" : "No");
        }
        lines.append(line.join(delimiter));
    }

    if (!copy) // export
    {
        QFile data(filePath);
        if (!data.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
        {
            QMessageBox::critical(parentWidget(), "Error!", "Could not save file, the file path cannot be opened!");
            return;
        }

        QTextStream out(&data);
        out << lines.join("\n");
    }
    else // clipboard
        QApplication::clipboard()->setText(lines.join("\n"));
}
