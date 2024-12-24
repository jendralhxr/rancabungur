#include "serslogger.h"

#define UPDATE_DELAY 10

SERSlogger::SERSlogger(QThread *parent) : QThread(parent)
{
}

SERSlogger::SERSlogger(const char *filename){
    fileLog.setFileName(filename);
    fileLog.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    stream.setDevice(&fileLog);
    stream << string.sprintf("another run at %s\n",QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss").toStdString().c_str());
    fileLog.close();

    this->start(QThread::LowPriority);
}

void SERSlogger::newAzimuth(double pos){
    latestAzimuth=pos;
}

void SERSlogger::newElevation(double pos){
    latestElevation=pos;
}

void SERSlogger::run(){
    while(1){
        // updating the file per update line
        fileLog.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        stream.setDevice(&fileLog);
        stream << string.sprintf("%lf %lf at %s",latestAzimuth,latestElevation,current_time.toString("dd/MM/yyyy hh:mm:ss").toStdString().c_str());
        stream << endl; // just to look wiser :)
        fileLog.close();
        qDebug("tambah record!!");
        sleep(UPDATE_DELAY);
    }
}
