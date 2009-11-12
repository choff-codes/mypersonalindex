#ifndef FRMOPTIONS_H
#define FRMOPTIONS_H

#include <QtGui>
#include "frmOptions_UI.h"
#include "queries.h"
#include "globals.h"

class frmOptions : public QDialog
{
    Q_OBJECT

public:
    const globals::settings& getReturnValues() const { return m_settings; }

    frmOptions(const globals::settings &s, const queries &sql, QWidget *parent = 0);

private:
    frmOptions_UI ui;
    globals::settings m_settings;
    globals::settings m_settingsOriginal;
    const queries &m_sql;

private slots:
    void accept();
};

#endif // FRMOPTIONS_H
