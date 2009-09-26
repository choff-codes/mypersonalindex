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

    frmOptions(QWidget *parent = 0, queries *sql = 0, const globals::settings& s = globals::settings());

private:
    frmOptions_UI ui;
    queries *m_sql;
    globals::settings m_settings;

private slots:
    void accept();
};

#endif // FRMOPTIONS_H
