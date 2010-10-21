#include "frmSort.h"

frmSort::frmSort(const QList<sort> &sort_, const QMap<int, QString> &columns_, QWidget *parent_):
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

    for (int i = 0; i < m_sort.count(); ++i)
    {
        sort s = m_sort.at(i);
        switch (i)
        {
            case 0:
                ui.sort1Cmb->setCurrentIndex(ui.sort1Cmb->findData(s.column));
                ui.group1SortDescRadio->setChecked(s.orderColumn == sort::order_descending);
                break;
            case 1:
                ui.sort2Cmb->setCurrentIndex(ui.sort2Cmb->findData(s.column));
                ui.group2SortDescRadio->setChecked(s.orderColumn == sort::order_descending);
                break;
            case 2:
                ui.sort3Cmb->setCurrentIndex(ui.sort3Cmb->findData(s.column));
                ui.group3SortDescRadio->setChecked(s.orderColumn == sort::order_descending);
                break;
        }
    }
}

void frmSort::accept()
{
    QList<sort> sortList;

    if (!ui.sort1Cmb->currentText().isEmpty())
        sortList.append(
            sort
            (
                ui.sort1Cmb->itemData(ui.sort1Cmb->currentIndex()).toInt(),
                ui.group1SortDescRadio->isChecked() ? sort::order_descending : sort::order_ascending
            )
        );

    if (!ui.sort2Cmb->currentText().isEmpty() && !sortList.isEmpty()) // make sure sort1 is set
        sortList.append(
            sort
            (
                ui.sort2Cmb->itemData(ui.sort2Cmb->currentIndex()).toInt(),
                ui.group2SortDescRadio->isChecked() ? sort::order_descending : sort::order_ascending
            )
        );

    if (!ui.sort3Cmb->currentText().isEmpty() && sortList.count() == 2) // make sure sort2 is set
        sortList.append(
            sort
            (
                ui.sort3Cmb->itemData(ui.sort3Cmb->currentIndex()).toInt(),
                ui.group3SortDescRadio->isChecked() ? sort::order_descending : sort::order_ascending
            )
        );

    if (sortList == m_sort)
    {
        QDialog::reject();
        return;
    }

    m_sort = sortList;
    QDialog::accept();
}
