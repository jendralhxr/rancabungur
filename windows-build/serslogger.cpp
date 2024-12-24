#include "serslogger.h"

#define UPDATE_DELAY_STANDBY 600 //secs
#define UPDATE_DELAY_BUSY 5 //secs

SERSlogger::SERSlogger(QThread *parent) : QThread(parent)
{
}

SERSlogger::SERSlogger(const char *filename){
    latestAzimuth=0;
    latestElevation=0;

    fileLog.setFileName(filename);
    fileLog.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    stream.setDevice(&fileLog);
    stream << string.sprintf("another run at %s\n",QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss").toStdString().c_str());
    fileLog.close();

    statusStandby();
}

void SERSlogger::newAzimuth(double pos){
    latestAzimuth=pos;
}

void SERSlogger::newElevation(double pos){
    latestElevation=pos;
}

void SERSlogger::statusBusy(){
    busy=true;
    //terminate();
    start(QThread::HighPriority);
}

void SERSlogger::statusStandby(){
    busy=false;
    //terminate();
    start(QThread::LowPriority);
}

void SERSlogger::run(){
    while(1){
        // updating the file per update line
        fileLog.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        stream.setDevice(&fileLog);
        stream << string.sprintf("%lf %lf at %s",latestAzimuth,latestElevation,QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss").toStdString().c_str());
        stream << endl; // just to look wiser :)
        fileLog.close();
        qDebug("%lf %lf tambah record!!",latestAzimuth,latestElevation);
        if (busy) sleep (UPDATE_DELAY_BUSY);
        else sleep(UPDATE_DELAY_STANDBY);
    }
}
