#include "mpiPlotZoomer.h"
#include <QDate>
#include "functions.h"

QwtText mpiPlotZoomer::trackerTextF(const QPointF &pos_) const
{
    QwtText text(
        QString("%1: %2").arg(
            QDate::fromJulianDay((int)pos_.x()).toString(Qt::SystemLocaleShortDate),
            functions::doubleToPercentage(pos_.y())
        )
    );

    QColor bg(Qt::white);
    bg.setAlpha(200);
    text.setBackgroundBrush(QBrush(bg));
    return text;
}
