#ifndef FRMCOMPARE_H
#define FRMCOMPARE_H

#include <QtGui>
#include "frmCompare_UI.h"
#include "portfolio.h"
#include "settings.h"
#include "mainCorrelationModel.h"
#include "mainStatisticModel.h"
#include "chartInfo.h"
#include "cachedCalculations.h"

class frmCompare : public QDialog
{
    Q_OBJECT

public:
    frmCompare(settings *parentSettings);
    ~frmCompare();

private:
    frmCompare_UI ui;
    settings *m_settings;
    QList<QwtPlotCurve*> m_curves;

    QHash<objectKey, navInfoStatistic> selected();
    void correlatation(const QHash<objectKey, navInfoStatistic> &items);
    void stat(const QHash<objectKey, navInfoStatistic> &items);
    void chart(const QHash<objectKey, navInfoStatistic> &items);

private slots:
    void refresh();
    void exportTab();
};

#endif // FRMCOMPARE_H
