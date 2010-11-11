#ifndef FRMMAINAA_UI_H
#define FRMMAINAA_UI_H

#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QShortcut>
#include <QAction>
#include "mpiDateEdit.h"
#include "mpiTableView.h"

class frmMainTableView_UI
{
public:
    QWidget *widget;
    QVBoxLayout *layout;
    QToolBar *toolbar;
    QLabel *toolbarDateBegin;
    mpiDateEdit *toolbarDateBeginEdit;
    QLabel *toolbarDateEnd;
    mpiDateEdit *toolbarDateEndEdit;
    QLabel *toolbarSort;
    QComboBox *toolbarSortCmb;
    QAction *toolbarReorder;
    QAction *toolbarExport;
    mpiTableView *table;
    QShortcut *tableCopy;

    void setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_ = 0);
};

#endif // FRMMAINAA_UI_H
