#include "serscontrol.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define POSITIONSCALING 100 // gearbox ratio
#define DRIVE_ADDRESS 1
#define ACCELMANUAL 5
#define ACCELPOSITION 5
#define VELOSLOW 150
#define VELOFAST 200
#define VELOPOSITION 180
#define UPDATEDELAY 2 //nice

SERScontrol::SERScontrol(QThread *parent) : QThread(parent)
{
}

SERScontrol::SERScontrol(const char *modem, double parking){
    parking_position = parking;

    rs232.setPortName(modem);
    rs232.open(QextSerialPort::ReadWrite);
    //rs232.setQueryMode(QextSerialPort::);
    rs232.setBaudRate(BAUD19200);
    rs232.setFlowControl(FLOW_OFF);
    rs232.setParity(PAR_NONE);
    rs232.setDataBits(DATA_8);
    rs232.setStopBits(STOP_1);

    // set velo dan accel
    rs232.write(string.sprintf("#%d\r",DRIVE_ADDRESS).toStdString().c_str()); // drive addr
    rs232.write(string.sprintf("#A=%d\r",ACCELPOSITION).toStdString().c_str()); // P138
    rs232.write(string.sprintf("#P1018=%d\r",ACCELMANUAL).toStdString().c_str());
    rs232.write(string.sprintf("#V=%d\r",VELOPOSITION).toStdString().c_str()); // P91
    rs232.write(string.sprintf("#P1019=%d\r",VELOSLOW).toStdString().c_str());
    rs232.write(string.sprintf("#P1020=%d\r",VELOFAST).toStdString().c_str());
    rs232.write(string.sprintf("#P1035=0\r").toStdString().c_str()); // no timeout jog
    rs232.write(string.sprintf("#P1134=1\r").toStdString().c_str()); // arah putar, lihat jumlah gearbox
    rs232.write(string.sprintf("#P1110=0\r").toStdString().c_str()); // wait until end of positioning=disable

    this->start(QThread::TimeCriticalPriority);
}

void SERScontrol::motorOn(){
    rs232.write("#ON\r");
}

void SERScontrol::motorOff(){
    rs232.write("#OFF\r");
}

void SERScontrol::jobStop(){
    if (mode==2){ // tracking->parking->standby
        jobParking();
        mode=0;
        }
    if (mode==1){ // manual->stop->standby
        rs232.write("#S\r");
        mode=0;
        }
}

void SERScontrol::positionInit(double pos){
    rs232.write(string.sprintf("#P51=%f\r",pos*POSITIONSCALING).toStdString().c_str());
}

void SERScontrol::positionExec(double pos){
    rs232.write(string.sprintf("#WA=%f E\r",pos*POSITIONSCALING).toStdString().c_str());
    mode=1;
}

void SERScontrol::jobParking(){ // parking
    rs232.write(string.sprintf("#WA=%f E\r",parking_position*POSITIONSCALING).toStdString().c_str());
    mode=1;
}

void SERScontrol::manualRightFast(){
    rs232.write(string.sprintf("#RF\r").toStdString().c_str());
    mode=1;
}

void SERScontrol::manualRightSlow(){
    rs232.write(string.sprintf("#RS\r").toStdString().c_str());
    mode=1;
}

void SERScontrol::manualLeftFast(){
    rs232.write(string.sprintf("#LF\r").toStdString().c_str());
    mode=1;
}

void SERScontrol::manualLeftSlow(){
    rs232.write(string.sprintf("#LS\r").toStdString().c_str());
    mode=1;
}

void SERScontrol::jobTracking(){
    mode=2;
}

void SERScontrol::run(){
    while(1){
        // tanya current position
        rs232.write("#P51?\r");
        rs232.readLine(buffer,sizeof(buffer));
        pch = NULL;
        pch = strstr(buffer,"P51");
        if (pch) sscanf(pch,"P51=%lf",&current_position);
        //printf("new current pos= %lf\n",current_position); // debugging
        current_position/=POSITIONSCALING;
        current_position=fmodl(current_position,360.0);
        if (current_position<0) emit updatePosition(current_position+360.0);
        else emit updatePosition(current_position);

        // debug here, please
        if (mode==1){
            rs232.write("#POS?\r"); // in possition?
            rs232.readLine(buffer,sizeof(buffer));
            pch = NULL;
            pch = strstr(buffer,"POS=1");
            if (pch) mode=0; // standby
        }

        if (mode==0) emit positionComplete();

        msleep(UPDATEDELAY);
    }
}
