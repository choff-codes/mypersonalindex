#include "frmEdit.h"

frmEdit::frmEdit(portfolio portfolio_, QWidget *parent):
    QDialog(parent),
    m_portfolio(portfolio_)
{
    ui.setupUI(this);
}
