#include "frmSort.h"

frmSort::frmSort(const QString &sort_, const QMap<int, QString> &columns_, QWidget *parent_):
    QDialog(parent_),
    m_sort(sort_),
    m_columns(columns_)
{
    ui.setupUI(this);

    connect(ui.okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.okCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));

    ui.sort1Cmb->addItem("", -1);
    ui.sort2Cmb->addItem("", -1);
    ui.sort3Cmb->addItem("", -1);

    for(QMap<int, QString>::const_iterator i = m_columns.constBegin(); i != m_columns.constEnd(); ++i)
    {
        ui.sort1Cmb->addItem(i.value(), i.key());
        ui.sort2Cmb->addItem(i.value(), i.key());
        ui.sort3Cmb->addItem(i.value(), i.key());
    }

    if (m_sort.isEmpty())
        return;

    QStringList list = m_sort.split('|');

    for (int i = 0; i < list.count(); ++i)
    {
        bool desc = list.at(i).at(0) == 'D'; // "D2" = column 2, descending, "2" would be column 2 ascending
        int sortColumn = desc ? list[i].remove(0, 1).toInt() : list.at(i).toInt();
        switch (i)
        {
            case 0:
                ui.sort1Cmb->setCurrentIndex(ui.sort1Cmb->findData(sortColumn));
                ui.group1SortDescRadio->setChecked(desc);
                break;
            case 1:
                ui.sort2Cmb->setCurrentIndex(ui.sort2Cmb->findData(sortColumn));
                ui.group2SortDescRadio->setChecked(desc);
                break;
            case 2:
                ui.sort3Cmb->setCurrentIndex(ui.sort3Cmb->findData(sortColumn));
                ui.group3SortDescRadio->setChecked(desc);
                break;
        }
    }
}

void frmSort::accept()
{
    QStringList sortList;

    if (!ui.sort1Cmb->currentText().isEmpty())
        sortList.append(
                (ui.group1SortDescRadio->isChecked() ? "D" : "") +
                QString::number(ui.sort1Cmb->itemData(ui.sort1Cmb->currentIndex()).toInt())
        );

    if (!ui.sort2Cmb->currentText().isEmpty() && !sortList.isEmpty()) // make sure sort1 is set
        sortList.append(
                (ui.group2SortDescRadio->isChecked() ? "D" : "") +
                QString::number(ui.sort2Cmb->itemData(ui.sort2Cmb->currentIndex()).toInt())
        );

    if (!ui.sort3Cmb->currentText().isEmpty() && sortList.count() == 2) // make sure sort2 is set
        sortList.append(
                (ui.group3SortDescRadio->isChecked() ? "D" : "") +
                QString::number(ui.sort2Cmb->itemData(ui.sort3Cmb->currentIndex()).toInt())
        );

    QString sortString = sortList.join("|");

    if (sortString == m_sort)
    {
        QDialog::reject();
        return;
    }

    m_sort = sortString;
    QDialog::accept();
}
