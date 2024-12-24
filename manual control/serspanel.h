#ifndef SERSPANEL_H
#define SERSPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <qwt_dial.h>
#include <qwt_dial_needle.h>

class SERSpanel : public QWidget
{
    Q_OBJECT
public:
    explicit SERSpanel(QWidget *parent = 0);
    SERSpanel(const char *nama, int origin, int value);

signals:
    void motorOn();
    void motorOff();
    void jobExec(int accel, int velo, double position);
    void JobStop();
    void manualRightFast(int accel, int velo);
    void manualRightSlow(int accel, int velo);
    void manualLeftFast(int accel, int velo);
    void manualLeftSlow(int accel, int velo);

public slots:
    void updateDial(double value);
    void statusBusy();
    void statusReady();
    // panel dari objek dalam
    void panelOn();
    void panelOff();
    void panelExecPos();
    void panelStop();
    void panelRightFast();
    void panelRightSlow();
    void panelLeftFast();
    void panelLeftSlow();

private:
    QLabel title;
    QLabel labelAccel;
    QLabel labelSpeed;
    QLabel labelPosition;
    QLabel labelJob;
    QLabel labelPower;
    QLabel labelStatus;
    QLabel labelManual;
    QLabel labelFast;
    QLabel labelSlow;
    QLabel labelPositioning;
    QLabel labelDrive;
    QLabel labelAccelManual;
    QPushButton buttonOn;
    QPushButton buttonOff;
    QPushButton buttonExec;
    QPushButton buttonStop;
    QPushButton buttonRightFast;
    QPushButton buttonRightSlow;
    QPushButton buttonLeftFast;
    QPushButton buttonLeftSlow;
    QDoubleSpinBox valPosition;
    QSpinBox valAccel;
    QSpinBox valSpeed;
    QSpinBox valSlow;
    QSpinBox valFast;
    QSpinBox valAccelManual;
    QwtDial dial;

    QVBoxLayout left;
    QVBoxLayout mid;
    QVBoxLayout right;
    QHBoxLayout all;

};

#endif // SERSPANEL_H
