#include "serscontrol.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// velocity in rpm
// acceleration in rad/s^2
// update delay in msec
#define POSITIONSCALING 100 // gearbox ratio
#define DRIVE_ADDRESS 1
#define ACCELMANUAL 5
#define ACCELPOSITION 5
#define VELOSLOW 140
#define VELOFAST 200
#define VELOPOSITION 180
#define UPDATEDELAY 5 //nice

SERScontrol::SERScontrol(QThread *parent) : QThread(parent)
{
}

SERScontrol::SERScontrol(const char *modem, double parking){
    parking_position = parking;

    rs232.setPortName(modem);
    rs232.open(Win_QextSerialPort::ReadWrite);
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

    for (int i=0; i<10; i++) rs232.readAll(); // being paranoid with rs232's buffer

    this->start(QThread::HighestPriority);
}

void SERScontrol::motorOn(){
    rs232.write("#ON\r");
}

void SERScontrol::motorOff(){
    rs232.write("#OFF\r");
}

void SERScontrol::jobStop(){
    rs232.write("#S\r");
    rs232.write("#S\r"); // being paranoid
    if (mode==2){ // tracking->parking->standby
        mode=1;
        jobParking();
        }
    if (mode==1){ // manual->stop->standby
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
        //qDebug("P51!!!");
        rs232.write("#P51?\r");
        rs232.readLine(buffer,sizeof(buffer));
        //qDebug("begin buffer: %s :end (raw) length=%d",buffer,strlen(buffer));
        if (strlen(buffer)>=10){
            pch = strstr(buffer,"P51=");
            if (pch) {
                sscanf(pch,"P51=%lf",&current_position);
                current_position/=POSITIONSCALING;
                current_position=fmod(current_position,360.0);
                qDebug("!!current pos: %lf; buffer length: %d\n",current_position,strlen(buffer));
                if (current_position<0) emit updatePosition(current_position+360.0);
                else emit updatePosition(current_position);
                }
             }

        // qDebug("mode = %d\n",mode);
        // in-position?
//        if (mode==1){
        if (mode!=2){
        //if (mode==1){
            rs232.write("#POS?\r"); // in possition?
            rs232.readLine(buffer,sizeof(buffer));
            if (strlen(buffer)==6){
                pch = strstr(buffer,"POS=1"); // diam;
                if (pch){
                    mode=0;
                    emit positionComplete();
                    // qDebug("pch0=%d; mode=%d\n",strlen(pch),mode);
                }
                pch = strstr(buffer,"POS=0"); // busy
                if (pch){
                    mode=1;
                    //qDebug("pch1=%d; mode=%d\n",strlen(pch),mode);
                }
            }
        }

        msleep(UPDATEDELAY);
    }
}
