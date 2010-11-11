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

    sort1Layout = new QGridLayout();
    sort1Cmb = new QComboBox(groupBox);
    group1SortAscRadio = new QRadioButton("Ascending", groupBox);
    group1SortAscRadio->setChecked(true);
    group1SortDescRadio = new QRadioButton("Descending", groupBox);
    group1 = new QButtonGroup(groupBox);
    group1->addButton(group1SortAscRadio);
    group1->addButton(group1SortDescRadio);
    sort1Layout->addWidget(sort1Cmb, 0, 0, 2, 1);
    sort1Layout->addWidget(group1SortAscRadio, 0, 1);
    sort1Layout->addWidget(group1SortDescRadio, 1, 1);
    groupLayout->addLayout(sort1Layout);

    sort2Layout = new QGridLayout();
    sort2Cmb = new QComboBox(groupBox);
    group2SortAscRadio = new QRadioButton("Ascending", groupBox);
    group2SortAscRadio->setChecked(true);
    group2SortDescRadio = new QRadioButton("Descending", groupBox);
    group2 = new QButtonGroup(groupBox);
    group2->addButton(group2SortAscRadio);
    group2->addButton(group2SortDescRadio);
    sort2Layout->addWidget(sort2Cmb, 0, 0, 2, 1);
    sort2Layout->addWidget(group2SortAscRadio, 0, 1);
    sort2Layout->addWidget(group2SortDescRadio, 1, 1);
    groupLayout->addLayout(sort2Layout);

    sort3Layout = new QGridLayout();
    sort3Cmb = new QComboBox(groupBox);
    group3SortAscRadio = new QRadioButton("Ascending", groupBox);
    group3SortAscRadio->setChecked(true);
    group3SortDescRadio = new QRadioButton("Descending", groupBox);
    group3 = new QButtonGroup(groupBox);
    group3->addButton(group3SortAscRadio);
    group3->addButton(group3SortDescRadio);
    sort3Layout->addWidget(sort3Cmb, 0, 0, 2, 1);
    sort3Layout->addWidget(group3SortAscRadio, 0, 1);
    sort3Layout->addWidget(group3SortDescRadio, 1, 1);
    groupLayout->addLayout(sort3Layout);

    layout->addWidget(groupBox);
    layout->addWidget(okCancelBtn);
    layout->setSizeConstraint(QLayout::SetFixedSize);
}
