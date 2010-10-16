#ifndef MPIVALIDATOR_H
#define MPIVALIDATOR_H

#include <QDoubleValidator>
#include <QIntValidator>

class mpiDoubleValidator: public QDoubleValidator
{

public:

    mpiDoubleValidator(double bottom_, double top_, int decimals_, QObject *parent_ = 0):
        QDoubleValidator(bottom_, top_, decimals_, parent_)
    {}

    QValidator::State validate(QString &input_, int &pos_) const;
};

class mpiIntValidator: public QIntValidator
{

public:

    mpiIntValidator(int bottom_, int top_, QObject *parent_ = 0):
        QIntValidator(bottom_, top_, parent_)
    {}

    QValidator::State validate(QString &input_, int &pos_) const;
};

#endif // MPIVALIDATOR_H
