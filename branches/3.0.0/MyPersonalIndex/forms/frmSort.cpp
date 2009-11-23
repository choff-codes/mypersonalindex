#include "frmSort.h"

frmSort::frmSort(const QString &sort, const QList<QString> &columns, QWidget *parent): QDialog(parent), m_sort(sort), m_columns(columns)
{
    ui.setupUI(this);

    ui.sort1->addItem("", -1);
    ui.sort2->addItem("", -1);
    ui.sort3->addItem("", -1);

    for(int i = 0; i < m_columns.count(); ++i)
        ui.sort1->addItem(m_columns.at(i), i);

    for(int i = 0; i < m_columns.count(); ++i)
        ui.sort2->addItem(m_columns.at(i), i);

    for(int i = 0; i < m_columns.count(); ++i)
        ui.sort3->addItem(m_columns.at(i), i);

    if (m_sort.isEmpty())
        return;

    QStringList list = m_sort.split(',');

    for (int i = 0; i < list.count(); ++i)
    {
        QStringList s = list.at(i).split(' ');
        switch (i)
        {
            case 0:
                ui.sort1->setCurrentIndex(ui.sort1->findData(s.at(0).toInt() - 1));  // remove the "DESC" from the values if it's attached
                if (s.count() == 2) // DESC attached at end
                    ui.rSortDesc1->setChecked(true);
                break;
            case 1:
                ui.sort2->setCurrentIndex(ui.sort2->findData(s.at(0).toInt() - 1));  // remove the "DESC" from the values if it's attached
                if (s.count() == 2) // DESC attached at end
                    ui.rSortDesc2->setChecked(true);
                break;
            case 2:
                ui.sort3->setCurrentIndex(ui.sort3->findData(s.at(0).toInt() - 1));  // remove the "DESC" from the values if it's attached
                if (s.count() == 2) // DESC attached at end
                    ui.rSortDesc3->setChecked(true);
                break;
        }
    }

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmSort::accept()
{
    QString sort;

    if (!ui.sort1->currentText().isEmpty())
        sort = QString::number(ui.sort1->itemData(ui.sort1->currentIndex()).toInt() + 1) + (ui.rSortDesc1->isChecked() ? " DESC" : "") +
            (ui.sort2->currentText().isEmpty() ? "" : "," + QString::number(ui.sort2->itemData(ui.sort2->currentIndex()).toInt() + 1) + (ui.rSortDesc2->isChecked() ? " DESC" : "")) +
            (ui.sort3->currentText().isEmpty() ? "" : "," + QString::number(ui.sort3->itemData(ui.sort3->currentIndex()).toInt() + 1) + (ui.rSortDesc3->isChecked() ? " DESC" : ""));

    if (sort == m_sort)
        QDialog::reject();
    else
    {
        m_sort = sort;
        QDialog::accept();
    }
}
