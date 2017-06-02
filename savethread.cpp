#include "savethread.h"
#include <QThread>
#include <QFile>
#include <QTextStream>

QThread thread4saving;
QString lokasi_simpan;
QString data_simpan;

saveThread::saveThread(QObject *parent) : QObject(parent)
{

}

void saveThread::getSaveLocation()
{
    //lokasi_simpan = QFileDialog::getSaveFileName(this,"Lokasi penyimpanan data...",QDir::currentPath(),"Log File (*.csv)");
}

void saveThread::begin(QString data4save)
{
    if (!thread4saving.isRunning()) {
        data_simpan=data4save;
        thread4saving.start();
    }
}

void saveThread::config()
{
    connect(&thread4saving,SIGNAL(started()),SLOT(doWork()));
    moveToThread(&thread4saving);
}

void saveThread::doWork()
{
    QFile outputFile(lokasi_simpan);
    if (!outputFile.isOpen()){
        outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    QTextStream outStream(&outputFile);
    outStream << data_simpan;
    outputFile.close();
    thread4saving.quit();
}


