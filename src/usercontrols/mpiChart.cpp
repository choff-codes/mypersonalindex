#include "mpiChart.h"
#include <QFileDialog>
#include <QMessageBox>

void mpiChart::exportChart()
{
    //http://www.qtcentre.org/threads/17616-Saving-qwt-plot-as-image?p=88077#post88077
    QPixmap qPix = QPixmap::grabWidget(this);

    QString fileType, filePath;
    filePath = QFileDialog::getSaveFileName(parentWidget(), "Export to...", QString(),
        "JPEG (*.jpeg);;PNG (*.png);;24-bit Bitmap (*.bmp)", &fileType);

    if (filePath.isEmpty())
        return;

    bool success = false;
    if (fileType.contains("JPEG"))
        success = qPix.save(filePath, "JPEG");
    else if (fileType.contains("PNG"))
        success = qPix.save(filePath, "PNG");
    else
        success = qPix.save(filePath, "BMP");

    if (!success)
        QMessageBox::critical(parentWidget(), "Error!", "Could not save file, the file path cannot be opened!");
}
