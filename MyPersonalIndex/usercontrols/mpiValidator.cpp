#include "mpiValidator.h"

QValidator::State mpiDoubleValidator::validate(QString &input_, int &pos_) const
{
    if (input_.isEmpty())
        return QValidator::Intermediate;

    if ( QDoubleValidator::validate(input_, pos_) != QValidator::Acceptable
         || input_ == "," || input_ == "." )  // has to be greater than 1
        return QValidator::Invalid;

    return QValidator::Acceptable;
}

QValidator::State mpiIntValidator::validate(QString &input_, int &pos_) const
{
    if (input_.isEmpty())
        return QValidator::Intermediate;

    if ( QIntValidator::validate(input_, pos_) != QValidator::Acceptable)
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
