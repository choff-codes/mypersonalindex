#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->statusBar->insertWidget(0, m_ui->progressBar, 0);
     m_ui->statusBar->insertWidget(0, m_ui->label_4, 1);
    m_ui->statusBar->insertWidget(0, m_ui->label_2, 0);
    m_ui->statusBar->insertWidget(0, m_ui->label, 0);
    m_ui->toolBar->insertWidget(m_ui->mainEdit, m_ui->label_5);
    m_ui->toolBar->insertWidget(m_ui->mainEdit, m_ui->comboBox);
    m_ui->toolBar->setContextMenuPolicy(Qt::NoContextMenu);
    m_ui->progressBar->setMaximum(0);
    m_ui->progressBar->setMaximum(100);
    holdings_bar = new QToolBar(m_ui->tab);
    m_ui->tab->layout()->addWidget(holdings_bar);
    m_ui->tab->layout()->removeWidget(m_ui->tableWidget);
    m_ui->tab->layout()->addWidget(m_ui->tableWidget);
    holdings_bar->setMovable(false);
    holdings_bar->setFloatable(false);
    holdings_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    holdings_bar->setIconSize(QSize(16, 16));
    holdings_bar->addAction(QIcon(":/icons/icons/options.png"),tr("Add..."));
    m_ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    for(int i = 0; i < m_ui->tableWidget->columnCount(); i++)
        m_ui->tableWidget->horizontalHeader()->setResizeMode(i, QHeaderView::Interactive);

    m_ui->tableWidget->insertRow(0);
    QCheckBox *check = new QCheckBox;
    m_ui->tableWidget->setCellWidget(0, 0, check);
    QTableWidgetItem *temp = new QTableWidgetItem("test", 0);
    m_ui->tableWidget->setItem(0, 1, temp);

    QWidget * w = new QWidget(m_ui->tableWidget);
    QHBoxLayout *l = new QHBoxLayout(w);
    l->setSpacing(0);
    l->setMargin(0);
    l->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    QCheckBox* check2 = new QCheckBox(w);
    check2->setChecked(true);
    connect(check2, SIGNAL(clicked(bool)), this, SLOT(checked(bool)));
    //check2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    l->addWidget(check2);
    l->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    w->setLayout(l);
    /*******/
    m_ui->tableWidget->setCellWidget(0, 2, w);

//    QTableWidgetItem *temp2 = new QTableWidgetItem(0);
//    temp2->setCheckState(Qt::Checked);
//    temp2->setFlags(Qt::ItemIsSelectable);
//    temp2->setTextAlignment(Qt::AlignCenter);
//    m_ui->tableWidget->setItem(0, 2, temp2);
}

void MainWindow::checked(bool checked)
{
    static_cast<QCheckBox*>(sender())->setChecked(!checked);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
