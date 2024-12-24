#ifndef SERSCONTROL_H
#define SERSCONTROL_H

#include <QThread>
#include <QString>
#include <win_qextserialport.h>
#include <stdio.h>

// velo in rpm
// accel in rad/s^2


class SERScontrol : public QThread{
    Q_OBJECT

public:
    explicit SERScontrol(QThread *parent = 0);
    SERScontrol(const char *modem, double parking);

signals:
    void updatePosition(double position);
    void positionComplete();

public slots:
    void motorOn();
    void motorOff();
    void jobStop(); // manual stop
    void jobParking(); // parking upon tracking completion
    void jobTracking();
    void positionInit(double pos);
    void positionExec(double pos);
    void manualLeftFast();
    void manualLeftSlow();
    void manualRightFast();
    void manualRightSlow();

private:
    void run();
    Win_QextSerialPort rs232;
    QString string;
    char buffer[32]; // or less
    double current_position, new_position;
    double parking_position;
    char *pch; // buat parsing string
    char mode; // 0=standby 1=manual 2=tracking
};

#endif // SERSCONTROL_H
