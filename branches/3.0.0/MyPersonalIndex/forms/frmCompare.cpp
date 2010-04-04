#include "frmCompare.h"

frmCompare::frmCompare()
{
    ui.setupUI(this);

    connect(ui.btnOk, SIGNAL(accepted()), this, SLOT(accept()));
}
