#include "sersschedule.h"
#include <math.h>
#include <QString>

#define pi 3.14159265359
#define EpochYear 2012
#define Epoch 059.71518596 // days of year
#define incl 97.6749 / 180 * pi //rad
#define RAAN 95.7810 / 180 * pi //rad
#define Ecc 0.0012407
#define Aop 313.8692 / 180 * pi // rad
#define Manom 46.1483 / 180 * pi // rad
#define MeanRev 1.805973222
#define Mmot 27743
#define TxtGsLat -6.5353932
#define TxtGsLong 106.7009085
#define TxtGsAlt 400 // meter above sea level
#define Mu 398600
#define J2 0.00108263
#define Re 6375
#define GMST0 100.553030 // by 2012

#define UPDATE_DELAY 5
#define FORESEE 15 // secs forward for calculation position

SERSschedule::SERSschedule(QThread *parent) :
    QThread(parent)
{
    newyear.setDate(QDate(EpochYear,1,1));
    newyear = newyear.toUTC();
}

void SERSschedule::jobTracking(){
    this->start(QThread::HighestPriority);
}

void SERSschedule::jobComplete(){
    this->terminate();
}

void SERSschedule::run(){
    double t; // day+fraction in year;
    double Dt, RevDt,  ResRevDt;
    double GMST;
    int FixRevDt;
    double a0, a1, n0, n0dot;
    double e0, e1, m;
    double phi;
    double RAANdot, NewRAAN;
    double Aopdot, NewAop;
    double theta;
    double coslamda1, sinlamda1, lamda1, lamda;
    double P, r, thetaG, lamdaG, Rg;
    double beta, L;
    double delta, sinalpha, cosalpha, alpha;
    QDateTime now;

    while(1){
        //hitung 'angka ajaib di orbit'
        now = QDateTime::currentDateTimeUtc();

        //t = (now.toMSecsSinceEpoch() - newyear.toMSecsSinceEpoch()) / 1000/366; // if leap year
        t = (((now.toMSecsSinceEpoch() - newyear.toMSecsSinceEpoch()) / 1000)+FORESEE) /365;

        //GMST
        GMST = GMST0 * pi / 180 + (360.98564735 * pi / 180 * t) - (2 * pi * int(t));
        GMST = (GMST / (2 * pi) - (int)(GMST / (2 * pi))) * 2 * pi;

        // time variable
        Dt = t - Epoch;
        RevDt = Dt * MeanRev;
        FixRevDt = (int) RevDt;
        ResRevDt = RevDt - FixRevDt;

        //SEMI MAJOR AXIS
        a0 = pow((Mu/pow(Mmot,2)),1/3);

        for (int i=0; i<10; i++){
            n0=sqrt(Mu/pow(a0,3)) * (1+3/2*J2*pow((Re/a0),2)*pow((1-pow(Ecc,2)),-1.5) * (1-3/2*pow(sin(incl),2)));
            n0dot = (-3/2)*pow(a0,5/2)*sqrt(Mu)-7/2*pow(a0,-9/2)*sqrt(Mu)*(3/2)*J2*pow(Re,2)*pow((1-pow(Ecc,2)),-1.5)*(1-3/2*pow(sin(incl),2));
            a1 = a0 + (Mmot-n0)/n0dot;
            a0 = a1;
        }

        // ANOMALY ECCENTRICITY (Newton-Raphson Iteration)
        m = Manom + ResRevDt*2*pi;
        if (((m<0) && (m>-pi)) || (m>pi)) e0 = m -Ecc;
        else e0=m+Ecc;

        e1=e0+(m-e0+Ecc*sin(e0))/(1-Ecc*cos(e0));

        for (char counter=0; counter<50; counter++){
            e0 = e1;
            e1 = e0 + (m - e0 + Ecc * sin(e0)) / (1 - Ecc * cos(e0));
            if((e0-e1)<=0.00000001) break;
            if((e1-e0)<=0.00000001) break;
        }

        //TRUE ANOMALY
        phi = 2 * atan(sqrt((1 + Ecc) / (1 - Ecc)) * tan(e1 / 2));

        //RIGHT ASCENSION OF THE ASCENDING NODE
        RAANdot = (-1.5) * J2 * MeanRev * pow((Re / a0),2) * pow((1 - pow(Ecc,2)),(-2)) * cos(incl);
        NewRAAN = RAAN + RAANdot * Dt * 2 * pi;

        //ARGUMENT OF PERIGEE
        Aopdot = (1.5) * J2 * MeanRev * pow((Re / a0),2) * pow((1 - pow(Ecc,2)),-2) * (2 - 5 / 2 * pow((sin(incl)),2));
        NewAop = Aop + Aopdot * Dt * 2 * pi;

        //LATITUDE
        theta = asin(sin(incl) * sin(NewAop + phi));

        //LONGITUDE

        //GMST = (GMST0(EpochYear) * pi / 180 + (360.98564735 * pi / 180 * t) - (2 * pi * Fix(t)))
        //GMST = (GMST / (2 * pi) - Fix(GMST / (2 * pi))) * 2 * pi
        // GMST!!
        coslamda1 = cos(NewAop + phi) / cos(theta);
        sinlamda1 = tan(theta) / tan(incl);
        lamda1 = atan2(sinlamda1, coslamda1);
        lamda = lamda1 + NewRAAN - GMST;
        lamda = (lamda / (2 * pi) - (int)(lamda / (2 * pi))) * 2 * pi;
        if (lamda<0) lamda = lamda + 2*pi;

        //RANGE
        P = a0 * (1 - pow(Ecc,2));
        r = P / (1 + Ecc * cos(phi));

        //TRACKING ANGLE
        thetaG = TxtGsLat * pi / 180;
        lamdaG = TxtGsLong * pi / 180;
        Rg = Re + (TxtGsAlt / 1000);

        //Slant Range
        beta = acos(sin(thetaG) * sin(theta) + (cos(thetaG) * cos(theta) * cos(2 * pi + lamda - lamdaG)));
        L = sqrt(pow(Rg,2) + pow(r,2) - 2 * r * Rg * cos(beta));

        //Elevation
        delta = asin((pow(r,2) - pow(Rg,2) - pow(L,2)) / (2 * Rg * L));

        // 'Azimuth
        sinalpha = sin(lamda - lamdaG) * cos(theta) / sin(beta);
        cosalpha = (sin(theta) - sin(thetaG) * cos(beta)) / (cos(thetaG) * sin(beta));
        alpha = atan2(sinalpha, cosalpha);
        if (alpha < 0) alpha = 2 * pi + alpha;

        //emit execAzimuth(scheduledAzimuth);
        //emit execElevation(scheduledElevation);
        //emit showScheduledAzimuth(foo);
        //emit showScheduledElevation(foo);
        if (delta<0) emit trackingOutofsight();
        else {
            scheduledAzimuth = alpha / pi * 180;
            scheduledElevation = delta / pi * 180;
            emit execAzimuth(scheduledAzimuth);
            emit execElevation(scheduledElevation);
            emit showScheduledAzimuth(string.sprintf("%lf",scheduledAzimuth).toStdString().c_str());
            emit showScheduledElevation(string.sprintf("%lf",scheduledElevation).toStdString().c_str());
            emit trackingLineofSight();
        }
        sleep(UPDATE_DELAY);
    }
}
