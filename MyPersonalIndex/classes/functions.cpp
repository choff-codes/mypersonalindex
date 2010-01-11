#include "functions.h"

QDateEdit* functions::createDateEdit(QWidget* parent)
{
    QDateEdit *d = new QDateEdit(parent);
    d->setDate(QDate::currentDate());
    d->setCalendarPopup(true);
    d->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
    return d;
}

void functions::showWelcomeMessage(QWidget *parent)
{
    QString welcomeMessage = "Welcome to My Personal Index!\n\nThere is no documentation yet,"
                             " but I recommend starting in the following way:\n\n1. Set the start date under options (on the top toolbar).\n"
                             "2. Add a new Portfolio\n3. Set your asset allocation \n4. Set your accounts\n5. Add holdings\n"
                             "6. Add relevant portfolio statistics\n7. Update prices!";
    QMessageBox msg(QMessageBox::Information, "My Personal Index", welcomeMessage, QMessageBox::Ok, parent);
    QPushButton *copyButton = msg.addButton("Copy To Clipboard", QMessageBox::RejectRole);

    msg.exec();

    if (msg.clickedButton() == copyButton)
        QApplication::clipboard()->setText(welcomeMessage);
}

QVariant functions::doubleToNull(const double &value)
{
    return value < 0 ? QVariant(QVariant::Double) : value;
}

QVariant functions::intToNull(const int &value)
{
    return value < 0 ? QVariant(QVariant::Int) : value;
}

QVariant functions::dateToNull(const int &value)
{
    return value == 0 ? QVariant(QVariant::Int) : value;
}

QString functions::doubleToCurrency(const double &value)
{
    if (value < 0)
        return QString("($%L1)").arg(-value, 0, 'f', 2);
    else
        return QString("$%L1").arg(value, 0, 'f', 2);
}

QString functions::doubleToPercentage(const double &value)
{
    return QString("%L1%").arg(value, 0, 'f', 2);
}

QString functions::doubleToLocalFormat(const double &value, const int &precision)
{
    return QString("%L1").arg(value, 0, 'f', precision);
}

double functions::stringToDouble(const QString &value, bool *ok)
{
    QString s = value;
    return s.remove('%').replace("None", "-1", Qt::CaseInsensitive).toDouble(ok);
}

void functions::exportTable(const QTableView *table, QMainWindow *parent)
{
    QString fileType, filePath;
    filePath = QFileDialog::getSaveFileName(parent, "Export to...", QDir::homePath(),
        "Tab Delimited File (*.txt);;Comma Delimited File (*.csv);;Pipe Delimited File (*.txt)", &fileType);
}

bool functions::lessThan(const QVariant &left, const QVariant &right, const QVariant &type)
{
    if ((left.isNull() && right.isNull()) || right.isNull())
        return false;

    if (left.isNull())
        return true;

    switch (type.type())
    {
        case QVariant::Double:
            return left.toDouble() < right.toDouble();
        case QVariant::Int:
            return left.toInt() < right.toInt();
        case QVariant::String:
        default:
            return left.toString().compare(right.toString(), Qt::CaseInsensitive) < 0;
    };
}

bool functions::greaterThan(const QVariant &left, const QVariant &right, const QVariant &type)
{
    if ((left.isNull() && right.isNull()) || left.isNull())
        return false;

    if (right.isNull())
        return true;

    switch (type.type())
    {
        case QVariant::Double:
            return left.toDouble() > right.toDouble();
        case QVariant::Int:
            return left.toInt() > right.toInt();
        case QVariant::String:
        default:
            return left.toString().compare(right.toString(), Qt::CaseInsensitive) > 0;
    };
}

bool functions::equal(const QVariant &left, const QVariant &right, const QVariant &type)
{
    if (left.isNull() && right.isNull())
        return true;

    switch (type.type())
    {
        case QVariant::Double:
            return left.toDouble() == right.toDouble();
        case QVariant::Int:
            return left.toInt() == right.toInt();
        case QVariant::String:
        default:
            return left.toString().compare(right.toString(), Qt::CaseInsensitive) == 0;
    };
}

QStringList functions::except(const QStringList &list1, const QStringList &list2)
{
    QStringList returnList;
    foreach(const QString &s, list1)
        if (!list2.contains(s))
            returnList.append(s);

    return returnList;
}
