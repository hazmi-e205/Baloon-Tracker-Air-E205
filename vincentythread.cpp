#include "vincentythread.h"
#include <QThread>
#include <math.h>

QThread thread4vincenty;
double PI = 3.14159265358979323846;
double LonLatAlt_v[5];
double *hasilVincenty;

vincentyThread::vincentyThread(QObject *parent) : QObject(parent)
{

}

double *vincentyThread::hitung(double lon1, double lat1, double lon2, double lat2, double alti)
{
    double a = 6378137, b = 6356752.314245, f = 1 / 298.257223563;
        double L = toRadians(lon2 - lon1);


        double U1 = atan((1 - f) * tan(toRadians(lat1)));
        double U2 = atan((1 - f) * tan(toRadians(lat2)));
        double sinU1 = sin(U1), cosU1 = cos(U1);
        double sinU2 = sin(U2), cosU2 = cos(U2);
        double cosSqAlpha;
        double sinSigma;
        double cos2SigmaM;
        double cosSigma;
        double sigma;
        double sinLambda,cosLambda;

        double lambda = L, lambdaP, iterLimit = 100;
        do
        {
            sinLambda = sin(lambda);
            cosLambda = cos(lambda);

            sinSigma = sqrt(	(cosU2 * sinLambda)
                            * (cosU2 * sinLambda)
                                + (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda)
                                    * (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda)
                                );
            if (sinSigma == 0)
            {
                return 0;
            }

            cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
            sigma = atan2(sinSigma, cosSigma);
            double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
            cosSqAlpha = 1 - sinAlpha * sinAlpha;
            cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;

            double C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
            lambdaP = lambda;
            lambda = 	L + (1 - C) * f * sinAlpha
                        * 	(sigma + C * sinSigma
                            * 	(cos2SigmaM + C * cosSigma
                                * 	(-1 + 2 * cos2SigmaM * cos2SigmaM)
                                )
                            );

        } while (abs(lambda - lambdaP) > 1e-12 && --iterLimit > 0);

        if (iterLimit == 0)
        {
            return 0;
        }

        double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
        double A = 1 + uSq / 16384
                * (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
        double B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
        double deltaSigma =
                    B * sinSigma
                        * (cos2SigmaM + B / 4
                            * (cosSigma
                                * (-1 + 2 * cos2SigmaM * cos2SigmaM) - B / 6 * cos2SigmaM
                                    * (-3 + 4 * sinSigma * sinSigma)
                                        * (-3 + 4 * cos2SigmaM * cos2SigmaM)));

        double s = b * A * (sigma - deltaSigma); //jarak meter

        //double initialBearing = atan2(cosU2*sinLambda,  cosU1*sinU2-sinU1*cosU2*cosLambda);
        double finalBearing = atan2(cosU1*sinLambda, -sinU1*cosU2+cosU1*sinU2*cosLambda);

        //initialBearing = (initialBearing + 2*Math.PI) % (2*Math.PI); // normalise to 0..360
        double finalBearing_ = fmod((finalBearing + (2*PI)),(2*PI)); // normalise to 0..360
                //(finalBearing + (2*PI))%(2*PI);

        double elevasi =atan2f(alti, s);



        static double hasil[3];
        hasil[0]=s;
        hasil[1]=toDegrees(finalBearing_);
        hasil[2]=toDegrees(elevasi);
        return hasil;
}


double vincentyThread::toRadians(double degree)
{
    double r = degree * PI / 180;
    return r;
}

double vincentyThread::toDegrees(double radian)
{
    double d = radian * 180 / PI;
    return d;
}



void vincentyThread::begin(double lon1, double lat1, double lon2, double lat2, double alti)
{
    LonLatAlt_v[0]=lon1;
    LonLatAlt_v[1]=lat1;
    LonLatAlt_v[2]=lon2;
    LonLatAlt_v[3]=lat2;
    LonLatAlt_v[4]=alti;
    if (!thread4vincenty.isRunning()) thread4vincenty.start();
}

void vincentyThread::config()
{
    connect(&thread4vincenty,SIGNAL(started()),SLOT(doWork()));
    moveToThread(&thread4vincenty);
}

void vincentyThread::doWork()
{
    hasilVincenty=hitung(LonLatAlt_v[0],LonLatAlt_v[1],LonLatAlt_v[2],LonLatAlt_v[3],LonLatAlt_v[4]);
    thread4vincenty.quit();
    emit selesai();
}

