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

    // blank
    foreach(QComboBox *combo, ui->sortCmb)
        combo->addItem("", -1);

    // actual values to sort by
    for(QMap<int, QString>::const_iterator i = m_columns.constBegin(); i != m_columns.constEnd(); ++i)
        foreach(QComboBox *combo, ui->sortCmb)
            combo->addItem(i.value(), i.key());

    // switch to any preselected choices
    for (int i = 0; i < m_orderBy.count(); ++i)
    {
        if (i == ui->sortCmb.count())
            break;

        orderBy o = m_orderBy.at(i);
        ui->sortCmb.at(i)->setCurrentIndex(ui->sortCmb.at(i)->findData(o.column));
        ui->sortDescRadio.at(i)->setChecked(o.direction == orderBy::order_descending);
    }
}

frmSort::~frmSort()
{
    delete ui;
}

void frmSort::accept()
{
    QList<orderBy> orderByList;

    for(int i = 0; i < ui->sortCmb.count(); ++i)
    {
        // make sure a sort in the middle isn't blank
        if (orderByList.count() != i)
            break;

        if (!ui->sortCmb.at(i)->currentText().isEmpty())
            orderByList.append(
                orderBy (
                    ui->sortCmb.at(i)->itemData(ui->sortCmb.at(i)->currentIndex()).toInt(),
                    ui->sortDescRadio.at(i)->isChecked() ? orderBy::order_descending : orderBy::order_ascending
                )
            );
    }

    if (orderByList == m_orderBy)
    {
        QDialog::reject();
        return;
    }

    m_orderBy = orderByList;
    QDialog::accept();
}
