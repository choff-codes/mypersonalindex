#ifndef MPITOOLBUTTON_H
#define MPITOOLBUTTON_H

#include <QtGui>

class mpiToolButton: public QToolButton
{
    Q_OBJECT

public:
    enum dropDownType { singleDate, startDate, endDate };

    mpiToolButton(QWidget *parent = 0, const dropDownType &type = singleDate): QToolButton(parent), m_type(type)
        { setPopupMode(QToolButton::InstantPopup); }
    void setDate(const QDate &date) {  m_date = date; }
    QDate date() const { return m_date; }
    dropDownType type() const { return m_type; }

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
     dropDownType m_type;
};

#endif // MPITOOLBUTTON_H
