#include "haversinethread.h"
#include <QThread>
#include <math.h>

QThread thread4haversine;
double LonLat_[4];
double arahAngin;
extern double PI;

haversineThread::haversineThread(QObject *parent) : QObject(parent)
{

}

double haversineThread::hitung(double lon1, double lat1, double lon2, double lat2)
{
    double a=toRadians(lon1), b=toRadians(lat1), c=toRadians(lon2), d=toRadians(lat2);
    double y = sin(c-a) * cos(d);
    double x = cos(b)*sin(d) - sin(b)*cos(d)*cos(c-a);
    double brng = (atan2(y, x));
    double brng1 = fmod((toDegrees(brng)+360) , 360);
    double brng2 = fmod(((brng1)+180) , 360);

    return brng2;
}

double haversineThread::toRadians(double degree)
{
    double r = degree * PI / 180;
    return r;
}

double haversineThread::toDegrees(double radian)
{
    double d = radian * 180 / PI;
    return d;
}

void haversineThread::begin(double lon1, double lat1, double lon2, double lat2)
{
    LonLat_[0]=lon1;
    LonLat_[1]=lat1;
    LonLat_[2]=lon2;
    LonLat_[3]=lat2;
    if (!thread4haversine.isRunning()) thread4haversine.start();
}


void haversineThread::config()
{
    connect(&thread4haversine,SIGNAL(started()),SLOT(doWork()));
    moveToThread(&thread4haversine);
}

void haversineThread::doWork()
{
    arahAngin=hitung(LonLat_[0],LonLat_[1],LonLat_[2],LonLat_[3]);
    thread4haversine.quit();
    emit selesai(arahAngin);
}
