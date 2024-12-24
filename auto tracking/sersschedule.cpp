#include "sersschedule.h"

#define UPDATE_DELAY 20

SERSschedule::SERSschedule(QThread *parent) :
    QThread(parent)
{
}

void SERSschedule::jobTracking(){
    this->start(QThread::NormalPriority);
}

void SERSschedule::jobComplete(){
    this->terminate();
}

void SERSschedule::run(){
    while(1){
        //hitung 'angka ajaib di orbit'
        //emit execAzimuth(scheduledAzimuth);
        //emit execElevation(scheduledElevation);
        //emit showScheduledAzimuth(foo);
        //emit showScheduledElevation(foo);
        sleep(UPDATE_DELAY);
    }
}
