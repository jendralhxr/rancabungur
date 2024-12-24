#include "serscontrol.h"
#define POSITIONSCALING 55.5556
#define UPDATEDELAY_BUSY 4
#define UPDATEDELAY_STANDBY 900

SERScontrol::SERScontrol(QThread *parent) : QThread(parent)
{
}

SERScontrol::~SERScontrol(){
    log.close();
    rs232.close();
}

SERScontrol::SERScontrol(const char *modem, const char *logfile, double pos_init){
    rs232.setPortName(modem);
    rs232.open(QextSerialPort::ReadWrite);
    /*
    rs232.setQueryMode(QextSerialPort::EventDriven);
    rs232.setBaudRate(BAUD56000);
    rs232.setFlowControl(FLOW_OFF);
    rs232.setParity(PAR_NONE);
    rs232.setDataBits(DATA_8);
    */

    current_time = QDateTime::currentDateTime();
    log.setFileName(logfile);
    log.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    stream.setDevice(&log);
    stream << string.sprintf("another run at %s\n",current_time.toString("dd/MM/yyyy hh:mm:ss").toStdString().c_str());
    log.close();

    // initial position (step) setting
    rs232.write(string.sprintf("#P51=%d\r",(int)(pos_init*POSITIONSCALING)).toStdString().c_str());
    emit updatePostion(pos_init);
}

void SERScontrol::run(){
    while(1){
        rs232.write("#P51?\r");
        rs232.read(buffer,64);
        sscanf(buffer,"POS = %d",&status);
        if (status) emit jobComplete();

        rs232.write("#POS?\r");
        rs232.read(buffer,64);
        sscanf(buffer,"P51 = %d",&position_current);
        emit updatePostion(position_current / POSITIONSCALING);

        position_current = 4;
        current_time = QDateTime::currentDateTime();

        log.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        stream.setDevice(&log);

        stream << string.sprintf("%f degree at %s",position_current/POSITIONSCALING,current_time.toString("dd/MM/yyyy hh:mm:ss").toStdString().c_str());
        stream << endl;
        log.close();

        if (status) sleep(UPDATEDELAY_STANDBY);
        else sleep(UPDATEDELAY_BUSY);

    }
}

void SERScontrol::motorOn(){
    rs232.write("#ON\r");
}

void SERScontrol::motorOff(){
    rs232.write("#OFF\r");
}

void SERScontrol::jobExec(int accel, int velo, double position){
    rs232.write(string.sprintf("#A=%d V=%d W=%d E\r",accel,velo,(int)(position * POSITIONSCALING)).toStdString().c_str());
    status = 1;
    emit jobBusy();
}

void SERScontrol::jobStop(){
    rs232.write("#S\r");
    status = 0;
    emit jobComplete();
}

void SERScontrol::manualRightFast(int accel, int velo){
    rs232.write(string.sprintf("P1018=%d P1020=%d RF\r",accel,velo).toStdString().c_str());
    status = 1;
    emit jobBusy();
}

void SERScontrol::manualRightSlow(int accel, int velo){
    rs232.write(string.sprintf("P1018=%d P1019=%d RS\r",accel,velo).toStdString().c_str());
    status = 1;
    emit jobBusy();
}

void SERScontrol::manualLeftFast(int accel, int velo){
    rs232.write(string.sprintf("P1018=%d P1020=%d LF\r",accel,velo).toStdString().c_str());
    status = 1;
    emit jobBusy();n
}

void SERScontrol::manualLeftSlow(int accel, int velo){
    rs232.write(string.sprintf("P1018=%d P1019=%d LS\r",accel,velo).toStdString().c_str());
    emit jobBusy();
}
