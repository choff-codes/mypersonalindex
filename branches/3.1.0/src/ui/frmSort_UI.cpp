#include "frmSort_UI.h"

void frmSort_UI::setupUI(QDialog *dialog_)
{
    dialog_->setWindowFlags(dialog_->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    dialog_->setWindowTitle("Custom Sort");

    okCancelBtn = new QDialogButtonBox(dialog_);
    okCancelBtn->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    layout = new QVBoxLayout(dialog_);
    groupBox = new QGroupBox("Columns", dialog_);
    groupLayout = new QVBoxLayout(groupBox);

    for(int i = 0; i < 3; ++i)
    {
        sortLayout.append(new QGridLayout());
        sortCmb.append(new QComboBox(groupBox));
        sortAscRadio.append(new QRadioButton("Ascending", groupBox));
        sortAscRadio.at(i)->setChecked(true);
        sortDescRadio.append(new QRadioButton("Descending", groupBox));
        sortButtonGroup.append(new QButtonGroup(groupBox));
        sortButtonGroup.at(i)->addButton(sortAscRadio.at(i));
        sortButtonGroup.at(i)->addButton(sortDescRadio.at(i));
        sortLayout.at(i)->addWidget(sortCmb.at(i), 0, 0, 2, 1);
        sortLayout.at(i)->addWidget(sortAscRadio.at(i), 0, 1);
        sortLayout.at(i)->addWidget(sortDescRadio.at(i), 1, 1);
        groupLayout->addLayout(sortLayout.at(i));
    }

    layout->addWidget(groupBox);
    layout->addWidget(okCancelBtn);
    layout->setSizeConstraint(QLayout::SetFixedSize);
}
