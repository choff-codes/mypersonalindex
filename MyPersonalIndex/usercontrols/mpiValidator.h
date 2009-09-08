#ifndef MPIVALIDATOR_H
#define MPIVALIDATOR_H

#include <QtGui>

class mpiDoubleValidator: public QDoubleValidator
{

public:

    mpiDoubleValidator(double bottom, double top, int decimals, QObject *parent = 0) : QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString &input, int &pos) const
    {
        if (input.isEmpty())
            return QValidator::Intermediate;

        if ( QDoubleValidator::validate(input, pos) != QValidator::Acceptable
             || input == "," || input == "." )  // has to be greater than 1
            return QValidator::Invalid;

        return QValidator::Acceptable;
    }

};

class mpiIntValidator: public QIntValidator
{

public:

    mpiIntValidator(double bottom, double top, QObject *parent = 0) : QIntValidator(bottom, top, parent) {}

    QValidator::State validate(QString &input, int &pos) const
    {
        if (input.isEmpty())
            return QValidator::Intermediate;

        if ( QIntValidator::validate(input, pos) != QValidator::Acceptable)
            return QValidator::Invalid;

        return QValidator::Acceptable;
    }

};

#endif // MPIVALIDATOR_H
