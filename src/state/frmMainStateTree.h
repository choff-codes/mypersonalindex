#ifndef FRMMAINSTATETREE_H
#define FRMMAINSTATETREE_H

template<class T>
class QList;
template<class T, class V>
class QHash;
class QTreeWidget;
class QTreeWidgetItem;
class QString;
class historicalNAV;
class portfolio;
class calculatorNAV;
class historicalPrices;
class frmMainStateTree
{
public:
    frmMainStateTree() {}
    virtual ~frmMainStateTree() {}

protected:
    virtual QTreeWidget* treeWidget() = 0;
    virtual QTreeWidgetItem* createTreeItem(int type_, const QString description_) = 0;
    virtual QTreeWidgetItem* createTreeItem(int type_, const QString description_, const QString itemData_) = 0;

    QList<QTreeWidgetItem*> selectedItems();

    void populateTree(const portfolio &portfolio_);
    virtual historicalNAV calculateNAV(QTreeWidgetItem *item_, int beginDate_, int endDate_, const portfolio &portfolio_,
        calculatorNAV calculator_, const QHash<QString, historicalPrices> prices_);
};

#endif // FRMMAINSTATETREE_H
