#include "frmSort_UI.h"

void frmSort_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    dialog->setWindowTitle("Custom Sort");

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    gpMain = new QGroupBox("Columns", dialog);
    groupLayout = new QVBoxLayout(gpMain);

    sort1Layout = new QGridLayout();
    sort1 = new QComboBox(gpMain);
    rSortAsc1 = new QRadioButton("Ascending", gpMain);
    rSortAsc1->setChecked(true);
    rSortDesc1 = new QRadioButton("Descending", gpMain);
    group1 = new QButtonGroup(gpMain);
    group1->addButton(rSortAsc1);
    group1->addButton(rSortDesc1);
    sort1Layout->addWidget(sort1, 0, 0, 2, 1);
    sort1Layout->addWidget(rSortAsc1, 0, 1);
    sort1Layout->addWidget(rSortDesc1, 1, 1);
    groupLayout->addLayout(sort1Layout);

    sort2Layout = new QGridLayout();
    sort2 = new QComboBox(gpMain);
    rSortAsc2 = new QRadioButton("Ascending", gpMain);
    rSortAsc2->setChecked(true);
    rSortDesc2 = new QRadioButton("Descending", gpMain);
    group2 = new QButtonGroup(gpMain);
    group2->addButton(rSortAsc2);
    group2->addButton(rSortDesc2);
    sort2Layout->addWidget(sort2, 0, 0, 2, 1);
    sort2Layout->addWidget(rSortAsc2, 0, 1);
    sort2Layout->addWidget(rSortDesc2, 1, 1);
    groupLayout->addLayout(sort2Layout);

    sort3Layout = new QGridLayout();
    sort3 = new QComboBox(gpMain);
    rSortAsc3 = new QRadioButton("Ascending", gpMain);
    rSortAsc3->setChecked(true);
    rSortDesc3 = new QRadioButton("Descending", gpMain);
    group3 = new QButtonGroup(gpMain);
    group3->addButton(rSortAsc3);
    group3->addButton(rSortDesc3);
    sort3Layout->addWidget(sort3, 0, 0, 2, 1);
    sort3Layout->addWidget(rSortAsc3, 0, 1);
    sort3Layout->addWidget(rSortDesc3, 1, 1);
    groupLayout->addLayout(sort3Layout);

    mainLayout->addWidget(gpMain);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}
