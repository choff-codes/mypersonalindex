#ifndef FRMCOLUMNS_H
#define FRMCOLUMNS_H

#include <QDialog>
#include <QMap>

class QListWidget;
class frmColumns_UI;
class frmColumns : public QDialog
{
    Q_OBJECT

public:
    QList<int> getReturnValues() const { return m_selectedItems; }

    frmColumns(const QList<int> &selectedItems_, const QMap<int, QString> &items_, QWidget *parent_ = 0);

    ~frmColumns();

private:
    enum direction {
        direction_up = -1,
        direction_down = 1
    };

    frmColumns_UI *ui;
    QList<int> m_selectedItems;
    QMap<int, QString> m_items;

    void switchSelected(QListWidget *from, QListWidget* to);
    void move(direction direction_);

private slots:
    void accept();
    void moveDown() { move(direction_down); }
    void moveUp() { move(direction_up); }
    void remove();
    void add();
};

#endif // FRMCOLUMNS_H
