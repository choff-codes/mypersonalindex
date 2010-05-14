#ifndef FRMCOLUMNS_H
#define FRMCOLUMNS_H

#include <QObject>
#include <QMap>
#include <QString>
#include "frmColumns_UI.h"
#include "settings.h"

class frmColumns : public QDialog
{
    Q_OBJECT

public:
    const QList<int>& getReturnValues() const { return m_selected; }

    frmColumns(const QList<int> &selected, const QMap<int, QString> &values, const QString &title,
        const QDialog::DialogCode &resultNoChange, QWidget *parent = 0);

private:
    frmColumns_UI ui;
    QList<int> m_selected;
    QMap<int, QString> m_values;
    QDialog::DialogCode m_resultNoChange;

    void switchSelected(QListWidget *from, QListWidget* to);

private slots:
    void accept();
    void moveDown();
    void moveUp();
    void remove() { switchSelected(ui.addedItems, ui.removedItems); }
    void add() { switchSelected(ui.removedItems, ui.addedItems); }
};

#endif // FRMCOLUMNS_H
