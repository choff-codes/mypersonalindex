#include "frmEditState.h"
#include <QDataStream>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>

const int frmEditState::m_magicNumber = qrand();

void frmEditState::copy()
{
    save();

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMimeData *mimeData = new QMimeData();

    stream << m_magicNumber << m_portfolio.id();
    if (!internalCopy(stream))
        return;

    mimeData->setData(mimeType(), data);
    QApplication::clipboard()->setMimeData(mimeData);
}

void frmEditState::paste()
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    QByteArray data;
    QDataStream stream(&data, QIODevice::ReadOnly);

    data = mimeData->data(mimeType());
    if (data.isEmpty())
        return;

    int magicNumber, portfolioID;
    stream >> magicNumber;
    stream >> portfolioID;

    if (magicNumber != m_magicNumber || portfolioID != m_portfolio.id())
        return;

    add();
    internalPaste(stream);
    load();
}
