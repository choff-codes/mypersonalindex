#ifndef FRMOPTIONS_H
#define FRMOPTIONS_H

#include <QtGui>
#include "frmOptions_UI.h"
#include "optionsQueries.h"
#include "globals.h"

class frmOptions : public QDialog
{
    Q_OBJECT

public:
    const globals::mpiSettings& getReturnValues() const { return m_settings; }

    frmOptions(QWidget *parent = 0, const globals::mpiSettings& settings = globals::mpiSettings());

private:
    frmOptions_UI ui;
    optionsQueries sql;
    globals::mpiSettings m_settings;

private slots:
    void accept();
};

#endif // FRMOPTIONS_H
