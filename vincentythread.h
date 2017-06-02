#ifndef VINCENTYTHREAD_H
#define VINCENTYTHREAD_H

#include <QObject>


class vincentyThread : public QObject
{
    Q_OBJECT
public:
    explicit vincentyThread(QObject *parent = 0);

    double *hitung (double lon1, double lat1, double lon2, double lat2, double alti);
    double toRadians(double degree);
    double toDegrees(double radian);

signals:
     void selesai();

public slots:
    void begin(double lon1, double lat1, double lon2, double lat2, double alti);
    void config();
    void doWork();
};

#endif // VINCENTYTHREAD_H
