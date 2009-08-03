#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>

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
