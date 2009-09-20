#ifndef FRMTABLEVIEWBASE_UI_H
#define FRMTABLEVIEWBASE_UI_H

#include <QtGui>

class frmTableViewBase_UI
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *gpMain;
    QGridLayout *groupLayout;
    QTableView *table;
    QVBoxLayout *groupLayoutRight;
    QPushButton *btnSelectAll;
    QPushButton *btnClear;
    QPushButton *btnMoveUp;
    QPushButton *btnMoveDown;
    QHBoxLayout *groupLayoutBottom;
    QPushButton *btnAdd;
    QPushButton *btnEdit;
    QPushButton *btnDelete;
    QPushButton *btnPaste;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog, const QString &groupBoxText, const bool &showPasteButton = false, const bool &showRightSideButtons = false);
};

#endif // FRMTABLEVIEWBASE_UI_H
