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
    QVBoxLayout *mainLayout;
    QGroupBox *gpMain;
    QHBoxLayout *groupLayout;
    QListWidget *removedItems;
    QListWidget *addedItems;
    QVBoxLayout *moveLayout;
    QToolButton *btnAdd;
    QToolButton *btnRemove;
    QVBoxLayout *reorderLayout;
    QToolButton *btnMoveUp;
    QToolButton *btnMoveDown;
    QDialogButtonBox *btnOkCancel;

    void setupUI(QDialog* dialog);
};

#endif // FRMCOLUMNS_UI_H
