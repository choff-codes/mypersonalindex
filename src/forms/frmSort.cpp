#include "frmSort.h"
#include "frmSort_UI.h"

frmSort::frmSort(const QList<orderBy> &orderBy_, const QMap<int, QString> &columns_, QWidget *parent_):
    QDialog(parent_),
    ui(new frmSort_UI),
    m_orderBy(orderBy_),
    m_columns(columns_)
{
    ui->setupUI(this);

    connect(ui->okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->okCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));

    ui->sort1Cmb->addItem("", -1);
    ui->sort2Cmb->addItem("", -1);
    ui->sort3Cmb->addItem("", -1);

    for(QMap<int, QString>::const_iterator i = m_columns.constBegin(); i != m_columns.constEnd(); ++i)
    {
        ui->sort1Cmb->addItem(i.value(), i.key());
        ui->sort2Cmb->addItem(i.value(), i.key());
        ui->sort3Cmb->addItem(i.value(), i.key());
    }

    for (int i = 0; i < m_orderBy.count(); ++i)
    {
        orderBy o = m_orderBy.at(i);
        switch (i)
        {
            case 0:
                ui->sort1Cmb->setCurrentIndex(ui->sort1Cmb->findData(o.column));
                ui->group1SortDescRadio->setChecked(o.direction == orderBy::order_descending);
                break;
            case 1:
                ui->sort2Cmb->setCurrentIndex(ui->sort2Cmb->findData(o.column));
                ui->group2SortDescRadio->setChecked(o.direction == orderBy::order_descending);
                break;
            case 2:
                ui->sort3Cmb->setCurrentIndex(ui->sort3Cmb->findData(o.column));
                ui->group3SortDescRadio->setChecked(o.direction == orderBy::order_descending);
                break;
        }
    }
}

frmSort::~frmSort()
{
    delete ui;
}

void frmSort::accept()
{
    QList<orderBy> orderByList;

    if (!ui->sort1Cmb->currentText().isEmpty())
        orderByList.append(
            orderBy
            (
                ui->sort1Cmb->itemData(ui->sort1Cmb->currentIndex()).toInt(),
                ui->group1SortDescRadio->isChecked() ? orderBy::order_descending : orderBy::order_ascending
            )
        );

    if (!ui->sort2Cmb->currentText().isEmpty() && !orderByList.isEmpty()) // make sure sort1 is set
        orderByList.append(
            orderBy
            (
                ui->sort2Cmb->itemData(ui->sort2Cmb->currentIndex()).toInt(),
                ui->group2SortDescRadio->isChecked() ? orderBy::order_descending : orderBy::order_ascending
            )
        );

    if (!ui->sort3Cmb->currentText().isEmpty() && orderByList.count() == 2) // make sure sort2 is set
        orderByList.append(
            orderBy
            (
                ui->sort3Cmb->itemData(ui->sort3Cmb->currentIndex()).toInt(),
                ui->group3SortDescRadio->isChecked() ? orderBy::order_descending : orderBy::order_ascending
            )
        );

    if (orderByList == m_orderBy)
    {
        QDialog::reject();
        return;
    }

    m_orderBy = orderByList;
    QDialog::accept();
}
