#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QMap>
#include "settings.h"
#include "portfolio.h"

class frmMain_UI;
class portfolio;
class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain();

private slots:
    void showPortfolioEdit();
    void about();

private:
    frmMain_UI *ui;
    QMap<int, portfolio> m_portfolios;
    portfolio m_portfolio;
    settings m_settings;
    QString m_fileLocation;

    void connectSlots();
    void closeEvent(QCloseEvent *event_);
    void loadSettings();
    void saveSettings();
};

#endif // FRMMAIN_H
