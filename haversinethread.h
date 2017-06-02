#ifndef HAVERSINETHREAD_H
#define HAVERSINETHREAD_H

#include <QObject>

class haversineThread : public QObject
{
    Q_OBJECT
public:
    explicit haversineThread(QObject *parent = 0);
    double hitung (double lon1, double lat1, double lon2, double lat2);
    double toRadians(double degree);
    double toDegrees(double radian);

signals:
     void selesai(int sdt_updt);

public slots:
    void begin(double lon1, double lat1, double lon2, double lat2);
    void config();
    void doWork();
};

#endif // HAVERSINETHREAD_H
