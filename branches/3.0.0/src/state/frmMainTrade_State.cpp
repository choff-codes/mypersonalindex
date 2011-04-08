#include "frmMainTrade_State.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "frmMainTreeView_UI.h"
#include "mainTradeModel.h"

frmMainTrade_State::frmMainTrade_State(const portfolio &portfolio_, QWidget *parent_):
    frmMainState(portfolio_, parent_),
    ui(new frmMainTreeView_UI())
{
    ui->setupUI(static_cast<QWidget*>(this->parent()));

    ui->toolbarDateBeginEdit->setDate(QDate::fromJulianDay(m_portfolio.startDate()));
    ui->toolbarDateEndEdit->setDate(QDate::fromJulianDay(m_portfolio.endDate()));

    connect(ui->toolbarDateBeginEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarDateEndEdit, SIGNAL(dateChanged(QDate)), this, SLOT(refreshTab()));
    connect(ui->toolbarExport, SIGNAL(triggered()), this, SLOT(exportTree()));

    refreshTab();
}

frmMainTrade_State::~frmMainTrade_State()
{
    delete ui;
}

QWidget* frmMainTrade_State::mainWidget()
{
    return ui->widget;
}

void frmMainTrade_State::refreshTab()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    QAbstractItemModel *model = ui->tree->model();
    ui->tree->setModel(createModel(ui->toolbarDateBeginEdit->date().toJulianDay(), ui->toolbarDateEndEdit->date().toJulianDay()));
    delete model;
#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (load tab)", t.elapsed());
#endif
}

QAbstractItemModel* frmMainTrade_State::createModel(int beginDate_, int endDate_)
{
    return new mainTradeModel(m_portfolio, beginDate_, endDate_, ui->tree);
}

void frmMainTrade_State::exportTree()
{
    QString fileType, filePath;
    QString delimiter = "\t";

    filePath = QFileDialog::getSaveFileName(static_cast<QWidget*>(this->parent()), "Export to...", QDir::homePath(),
        "Tab Delimited File (*.txt);;Comma Delimited File (*.csv);;Pipe Delimited File (*.txt)", &fileType);

    if (filePath.isEmpty())
        return;

    delimiter = fileType.contains("Tab") ? "\t" : fileType.contains("Pipe") ? "|" : ",";

    QStringList output;
    QAbstractItemModel *model = ui->tree->model();
    for(int i = 0; i < model->rowCount(); ++i)
    {
        QModelIndex sec = model->index(i, 0);
        output.append(model->data(sec).toString().remove(delimiter));
        for(int x = 0; x < model->rowCount(sec); ++x)
            output.append(delimiter + model->data(model->index(x, 0, sec)).toString().remove(delimiter));
    }

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
    {
        QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Error!", "Could not save file, the file path cannot be opened!");
        return;
    }

    QTextStream out(&file);
    out << output.join("\n");
}
