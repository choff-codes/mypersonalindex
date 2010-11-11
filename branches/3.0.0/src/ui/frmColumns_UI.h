#ifndef FRMCOLUMNS_UI_H
#define FRMCOLUMNS_UI_H

#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QToolButton>
#include <QDialogButtonBox>
#include <QDialog>

class frmColumns_UI
{
public:
    QVBoxLayout *layout;
    QGroupBox *groupBox;
    QHBoxLayout *groupBoxLayout;
    QListWidget *removedItemsList;
    QListWidget *addedItemsList;
    QVBoxLayout *moveLayout;
    QToolButton *moveBtnAdd;
    QToolButton *moveBtnRemove;
    QVBoxLayout *reorderLayout;
    QToolButton *reorderBtnMoveUp;
    QToolButton *reorderBtnMoveDown;
    QDialogButtonBox *okCancelBtn;

    void setupUI(QDialog *dialog_);
};

#endif // FRMCOLUMNS_UI_H
