#ifndef MPIVALIDATOR_H
#define MPIVALIDATOR_H

#include <QtGui>

class mpiDoubleValidator: public QDoubleValidator
{

public:

    mpiDoubleValidator(double bottom, double top, int decimals, QObject *parent = 0) : QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString &input, int &pos) const;
};

class mpiIntValidator: public QIntValidator
{

public:

    mpiIntValidator(int bottom, int top, QObject *parent = 0) : QIntValidator(bottom, top, parent) {}

    QValidator::State validate(QString &input, int &pos) const;
};

#endif // MPIVALIDATOR_H
