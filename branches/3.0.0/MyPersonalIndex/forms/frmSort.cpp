#include "frmSort.h"

frmSort::frmSort(const QString &sort, const QMap<int, QString> &columns, QWidget *parent): QDialog(parent), m_sort(sort), m_columns(columns)
{
    ui.setupUI(this);

    ui.sort1->addItem("", -1);
    ui.sort2->addItem("", -1);
    ui.sort3->addItem("", -1);

    for(QMap<int, QString>::const_iterator i = m_columns.constBegin(); i != m_columns.constEnd(); ++i)
    {
        ui.sort1->addItem(i.value(), i.key());
        ui.sort2->addItem(i.value(), i.key());
        ui.sort3->addItem(i.value(), i.key());
    }

    if (m_sort.isEmpty())
        return;

    QStringList list = m_sort.split('|');

    for (int i = 0; i < list.count(); ++i)
    {
        bool desc = list.at(i).at(0) == 'D';
        int sortColumn = desc ? QString(list.at(i)).remove(0, 1).toInt() : list.at(i).toInt();
        switch (i)
        {
            case 0:
                ui.sort1->setCurrentIndex(ui.sort1->findData(sortColumn));
                ui.rSortDesc1->setChecked(desc);
                break;
            case 1:
                ui.sort2->setCurrentIndex(ui.sort2->findData(sortColumn));
                ui.rSortDesc2->setChecked(desc);
                break;
            case 2:
                ui.sort3->setCurrentIndex(ui.sort3->findData(sortColumn));
                ui.rSortDesc3->setChecked(desc);
                break;
        }
    }

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmSort::accept()
{
    QStringList sortList;
    QString sortString;

    if (!ui.sort1->currentText().isEmpty())
    {
        sortList.append(
                (ui.rSortDesc1->isChecked() ? "D" : "") +
                QString::number(ui.sort1->itemData(ui.sort1->currentIndex()).toInt())
        );

        if (!ui.sort2->currentText().isEmpty())
            sortList.append(
                    (ui.rSortDesc2->isChecked() ? "D" : "") +
                    QString::number(ui.sort2->itemData(ui.sort2->currentIndex()).toInt())
            );

        if (!ui.sort3->currentText().isEmpty())
            sortList.append(
                    (ui.rSortDesc3->isChecked() ? "D" : "") +
                    QString::number(ui.sort2->itemData(ui.sort3->currentIndex()).toInt())
            );

        sortString = sortList.join("|");
    }

    if (sortString == m_sort)
        QDialog::reject();
    else
    {
        m_sort = sortString;
        QDialog::accept();
    }
}
