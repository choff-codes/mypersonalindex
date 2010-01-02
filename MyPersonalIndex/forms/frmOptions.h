#ifndef FRMOPTIONS_H
#define FRMOPTIONS_H

#include <QtGui>
#include "frmOptions_UI.h"
#include "queries.h"

class frmOptions : public QDialog
{
    Q_OBJECT

public:
    const settings& getReturnValues() const { return m_settings; }

    frmOptions(const settings &s, QWidget *parent = 0);

private:
    frmOptions_UI ui;
    settings m_settings;
    settings m_settingsOriginal;

private slots:
    void accept();
};

#endif // FRMOPTIONS_H
