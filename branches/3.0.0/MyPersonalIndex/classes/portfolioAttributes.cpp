#include "portfolioAttributes.h"

portfolioAttributes::portfolioAttributes(int id_, const QString description_):
    objectKey(description_, id_),
    startValue(100),
    startDate(QDate::currentDate().toJulianDay())
{}

bool portfolioAttributes::operator==(const portfolioAttributes &other_) const
{
    return objectKey::operator ==(other_)
            && this->startValue == other_.startValue
            && this->startDate == other_.startDate;
}

void portfolioAttributes::save(const queries &dataSource_)
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_Description), description);
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_StartValue), startValue);
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_StartDate), startDate);

    this->id = dataSource_.insert(queries::table_Portfolio, values, this->id);
}

void portfolioAttributes::remove(const queries &dataSource_) const
{
    if (!hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_Portfolio, this->id);
}

portfolioAttributes portfolioAttributes::load(const QSqlQuery &q_)
{
    portfolioAttributes p(
        q_.value(queries::portfolioColumns_ID).toInt(),
        q_.value(queries::portfolioColumns_Description).toString()
    );

    p.startDate = q_.value(queries::portfolioColumns_StartDate).toInt();
    p.startValue = q_.value(queries::portfolioColumns_StartValue).toInt();

    return p;
}

QString portfolioAttributes::validate() const
{
    if (this->description.isEmpty())
        return "Please enter a description!";

    if (this->startValue < 1 || this->startValue > 1000000)
        return "Beginning NAV must be between 1 and 1000000!";

    return QString();
}
