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
    void open();
    bool save();
    bool saveAs();
    void newFile();
    void recentFileSelected();

private:
    frmMain_UI *ui;
    QMap<int, portfolio> m_portfolios;
    portfolio m_portfolio;
    settings m_settings;

    void connectSlots();
    void closeEvent(QCloseEvent *event_);
    void loadSettings();
    void saveSettings();
    void loadFile(const QString &filePath_);
    void setCurrentFile(const QString &filePath_);
    bool maybeSave();
    bool saveFile(const QString &filePath_);
    void updateRecentFileActions();
};

#endif // FRMMAIN_H
