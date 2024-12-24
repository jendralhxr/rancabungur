#ifndef SERSCONTROL_H
#define SERSCONTROL_H

#include <QThread>
#include <QFile>
#include <QDateTime>
#include <QString>
#include <QTextStream>
#include <qextserialport.h>
#include <stdio.h>

class SERScontrol : public QThread{
    Q_OBJECT

public:
    explicit SERScontrol(QThread *parent = 0);
    SERScontrol(const char *modem, const char *logfile, double pos_init);
    ~SERScontrol();

signals:
    void updatePostion(double position);
    void jobComplete();
    void jobBusy();

public slots:
    void motorOn();
    void motorOff();
    void jobExec(int accel, int velo, double position);
    void jobStop();
    void manualRightFast(int accel, int velo);
    void manualRightSlow(int accel, int velo);
    void manualLeftFast(int accel, int velo);
    void manualLeftSlow(int accel, int velo);

private:
    void run();
    QFile log;
    QDateTime current_time;
    QextSerialPort rs232;
    QString string;
    QTextStream stream;
    int position_current;
    int status; // 1=standby 0=busy
    char buffer[64];
};

#endif // SERSCONTROL_H
