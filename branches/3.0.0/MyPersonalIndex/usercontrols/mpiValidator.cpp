#include "mpiValidator.h"

QValidator::State mpiDoubleValidator::validate(QString &input, int &pos) const
{
    if (input.isEmpty())
        return QValidator::Intermediate;

    if ( QDoubleValidator::validate(input, pos) != QValidator::Acceptable
         || input == "," || input == "." )  // has to be greater than 1
        return QValidator::Invalid;

    return QValidator::Acceptable;
}

QValidator::State mpiIntValidator::validate(QString &input, int &pos) const
{
    if (input.isEmpty())
        return QValidator::Intermediate;

    if ( QIntValidator::validate(input, pos) != QValidator::Acceptable)
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
