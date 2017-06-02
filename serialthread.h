#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QSerialPort>



class serialThread : public QObject
{
    Q_OBJECT
public:
    explicit serialThread(QObject *parent = 0);

    //QSerialPort *serialOne;
    QSerialPort serialOne;
    QString dataAll;
    QStringList pisah;
    bool lanjut=false;
    float cacah,perdetik;

signals:
    void dataPenuh(QString,int);
    void dataPisah(QStringList);


public slots:
    void begin(QString namaPort);
    void stopAll();
    void config();
    void doWork();
    void penghitung();


};

#endif // SERIALTHREAD_H
