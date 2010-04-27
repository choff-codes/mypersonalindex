#ifndef FRMCOMPARE_H
#define FRMCOMPARE_H

#include <QtGui>
#include "frmCompare_UI.h"
#include "portfolio.h"
#include "mainCorrelationModel.h"
#include "cachedCalculations.h"
#include "settings.h"

class frmCompare : public QDialog
{
    Q_OBJECT

public:
    frmCompare(settings *parentSettings);
    ~frmCompare();

private:
    frmCompare_UI ui;
    settings *m_settings;

    QHash<objectKey, navInfoStatistic> selected();

private slots:
    void correlatation();
};

#endif // FRMCOMPARE_H
