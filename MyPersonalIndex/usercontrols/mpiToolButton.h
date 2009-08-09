#ifndef MPITOOLBUTTON_H
#define MPITOOLBUTTON_H

#include <QtGui>

class mpiToolButton: public QToolButton
{
    Q_OBJECT

public:
    mpiToolButton(QWidget *parent = 0): QToolButton(parent) {}
    QDate currentDate;

signals:
    void pressDown();

protected:
    void mousePressEvent(QMouseEvent* e)  // virtual
    {
      emit pressDown();

      // call the base method
      QToolButton::mousePressEvent(e);
    }
};

#endif // MPITOOLBUTTON_H
