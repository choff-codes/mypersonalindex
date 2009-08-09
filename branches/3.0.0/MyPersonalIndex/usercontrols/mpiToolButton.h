#ifndef MPITOOLBUTTON_H
#define MPITOOLBUTTON_H

#include <QtGui>

class mpiToolButton: public QToolButton
{
    Q_OBJECT

public:
    enum DropDownType { SingleDate, StartDate, EndDate };

    mpiToolButton(QWidget *parent = 0, DropDownType Type = SingleDate): QToolButton(parent), m_Type(Type)
        { setPopupMode(QToolButton::InstantPopup); }
    void setDate(const QDate &date) {  m_date = date; }
    QDate date() const { return m_date; }
    DropDownType Type() const { return m_Type; } 

signals:
    void pressDown();

protected:
    void mousePressEvent(QMouseEvent* e)  // virtual
    {
      emit pressDown();

      // call the base method
      QToolButton::mousePressEvent(e);
    }

 private:
     QDate m_date;
     DropDownType m_Type;
};

#endif // MPITOOLBUTTON_H
