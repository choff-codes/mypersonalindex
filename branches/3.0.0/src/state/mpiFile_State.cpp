#include "mpiFile_State.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QFileDialog>
#include "queries.h"
#include "portfolioFactory.h"
#include "priceFactory.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

mpiFile_State::mpiFile_State(QWidget *parent_) :
    QObject(parent_)
{
}

QWidget* mpiFile_State::parent() const
{
    return static_cast<QWidget*>(QObject::parent());
}

void mpiFile_State::newFile()
{
    if (!maybeSave())
        return;

    portfolios.clear();
    prices.clear();
    portfolioIdentities.clear();
    setCurrentFile("", true);
}

bool mpiFile_State::maybeSave()
{
    if (!modified)
        return true;

    QMessageBox::StandardButton ret = QMessageBox::warning(this->parent(), QCoreApplication::applicationName(), "The file has been modified.\n\n"
        "Do you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (ret == QMessageBox::Save)
        return save();

     if (ret == QMessageBox::Cancel)
         return false;

     return true;
}

bool mpiFile_State::save()
{
    if (m_filePath.isEmpty())
        return saveAs();

    return saveFile(m_filePath);
}

bool mpiFile_State::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this->parent(), "Save As...", QString(), "My Personal Index File (*.mpi);;All Files (*)");

    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool mpiFile_State::saveFile(const QString &filePath_)
{
    if (!prepareFileForSave(filePath_))
        return false;

    queries file(filePath_);
    if (!file.isValid())
    {
        QMessageBox::critical(this->parent(), QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return false;
    }
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    file.beginTransaction();

    portfolioIdentities = portfolio::save(portfolios, file);
    file.commit();

#ifdef CLOCKTIME
    qDebug("Time elapsed (save porfolios): %d ms", t.elapsed());
#endif
    file.beginTransaction();

    prices.save(file);

    file.commit();
#ifdef CLOCKTIME
    qDebug("Time elapsed (save prices): %d ms", t.elapsed());
#endif

    setCurrentFile(filePath_, false);
    return true;
}

bool mpiFile_State::prepareFileForSave(const QString &filePath_)
{
    if (m_filePath == filePath_)
        return true;

    if (QFile::exists(filePath_) && !QFile::remove(filePath_)) {
        QMessageBox::warning(this->parent(), QCoreApplication::applicationName(), QString("Could not overwrite the existing file %1!").arg(filePath_));
        return false;
    }

    if (m_filePath.isEmpty()) // new file
    {
        if (!QFile::copy("MPI.sqlite", filePath_))
        {
            QMessageBox::warning(this->parent(), QCoreApplication::applicationName(), QString("Could not save to %1!").arg(filePath_));
            return false;
        }
    }
    else
    {
        if (!QFile::copy(m_filePath, filePath_))
        {
            QMessageBox::warning(this->parent(), QCoreApplication::applicationName(),
                QString("Could not save to %1 OR the original file was deleted at %2!").arg(filePath_, m_filePath));
            return false;
        }
    }

    return true;
}

void mpiFile_State::open(const QString &filePath_)
{
    if(!maybeSave())
        return;

    loadFile(filePath_);
}

void mpiFile_State::open()
{
    if(!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this->parent(), "Open file...", QString(), "My Personal Index File (*.mpi);;All Files (*)");
    if (filePath.isEmpty())
        return;

    loadFile(filePath);
}

void mpiFile_State::loadFile(const QString &filePath_)
{
    queries file(filePath_);
    if (!file.isValid())
    {
        QMessageBox::critical(this->parent(), QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return;
    }

    prices = priceFactory(file).getHistoricalPrices();
    portfolios = portfolioFactory(file).getPortfolios();
    portfolioIdentities.clear();

    setCurrentFile(filePath_, true);
}

void mpiFile_State::setCurrentFile(const QString &filePath_, bool newFile_)
{
    m_filePath = filePath_;
    modified = false;
    emit fileNameChange(filePath_, newFile_);
}
