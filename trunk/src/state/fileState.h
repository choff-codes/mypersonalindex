#ifndef FILESTATE_H
#define FILESTATE_H

#include <QMap>
#include <portfolio.h>
#include "historicalPrices.h"
#include "fileStateIdentity.h"

class fileState : public QObject
{
    Q_OBJECT
public:
    fileState(QWidget *parent_ = 0);

    // modified?
    bool modified;

    // all portfolios in the current file
    QMap<int, portfolio> portfolios;

    // track new identities
    fileStateIdentity identities;

    // all prices in the current file
    historicalPricesMap prices;

    void open(const QString &filePath_, bool pricing_ = true);
    bool maybeSave();
    QString path() const { return m_filePath; }

signals:
    void fileNameChange(const QString &filePath_, bool newFile_);

public slots:
    void open(bool pricing_ = true);
    bool save();
    bool saveAs();
    void newFile();

private:
    // the current file path (empty is a new file)
    QString m_filePath;

    // the path of the sqlite file
    QString DATABASE_LOCATION;

    void loadFile(const QString &filePath_, bool pricing_);
    void setCurrentFile(const QString &filePath_, bool newFile_);
    bool saveFile(const QString &filePath_);
    bool prepareFileForSave(const QString &filePath_);
    QWidget* parent() const;

    QString checkDatabaseVersion(const QString &filePath_);
    void upgradeVersion300(queries file_);
};

#endif // FILESTATE_H
