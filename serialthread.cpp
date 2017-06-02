#include "serialthread.h"
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QDebug>

QThread thread4serial;
QTimer timer12;

bool baca=false;
char buffer[50];
float *dataArray;


serialThread::serialThread(QObject *parent) : QObject(parent)
{
}

void serialThread::begin(QString namaPort)
{
    if (!serialOne.isOpen()){
        serialOne.setPortName(namaPort);
        serialOne.setBaudRate(QSerialPort::Baud115200);
        serialOne.setDataBits(QSerialPort::Data8);
        serialOne.setParity(QSerialPort::NoParity);
        serialOne.setStopBits(QSerialPort::OneStop);
        serialOne.setFlowControl(QSerialPort::NoFlowControl);
        serialOne.open(QIODevice::ReadWrite);
        lanjut=true;
        if (serialOne.isOpen()){
            if (!thread4serial.isRunning()) {
                thread4serial.start();

                connect(&serialOne,SIGNAL(readyRead()),this,SLOT(doWork()));
                timer12.start(1000);
            }
        }
    }
}

void serialThread::stopAll()
{
    lanjut = false;
    thread4serial.quit();
    while (thread4serial.isRunning()){}
    if (!thread4serial.isRunning()) {
        if (serialOne.isOpen()){
            serialOne.close();
            timer12.stop();
        }
    }
}

void serialThread::config()
{
    //connect(&serialOne,SIGNAL(readyRead()),this,SLOT(doWork()));
    connect(&thread4serial,SIGNAL(started()),this,SLOT(doWork()));
    connect(&timer12,SIGNAL(timeout()),this,SLOT(penghitung()));
    moveToThread(&thread4serial);
}

unsigned int kirimSignal=0;

QString dataSemua;
void serialThread::doWork()
{
    QString terima = serialOne.readLine();
    QStringList terima1 = terima.split('@');
    if (terima1.length()==2){
        QStringList terima2 = terima1[1].split('#');
        if (terima2.length()==2){
            emit dataPenuh(terima2[0],perdetik);
            qDebug()<< "Serial Thread ID is " <<QThread::currentThreadId();
            cacah++;
        }
    }

    /*
        QString terima = serialOne.read(1);

        if (terima=="@") {
            dataSemua="";
            baca=true;
        }

        else if (terima=="#"){
            emit dataPenuh(dataSemua,perdetik);
            qDebug()<< "Serial Thread ID is " <<QThread::currentThreadId();
            cacah++;
            baca=false;
        }

        else {
            if (baca==true) dataSemua += terima;
        }
    */
}

void serialThread::penghitung()
{
    perdetik=cacah;
    cacah=0;
}






