#ifndef SERSPANEL_H
#define SERSPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

#include <qwt_dial.h>
#include <qwt_dial_needle.h>


class SERSpanel : public QWidget
{
    Q_OBJECT
public:
    explicit SERSpanel(QWidget *parent = 0);
    SERSpanel(const char *title, int origin, double max);

signals:
    void motorOn();
    void motorOff();
    void positionTrack(); // tracking
    void positionInit(double pos); // position init
    void positionExec(double pos); // manual positioning
    void positionStop(); // STOP (tracking, manual drive or positioning)
    void manualLeftFast();
    void manualLeftSlow();
    void manualRightFast();
    void manualRightSlow();

public slots:
    void updateDial(double value);
    void updateSchedule(const char *string);
    void statusStandby();
    void statusBusy();
    void statusTrackingInsight();
    void statusTrackingOutsight();


// panel dari objek dalam
private slots:
    void panelOn();
    void panelOff();
    void panelTrack();
    void panelStop();
    void panelPositionInit();
    void panelPositionManual();
    void panelLeftFast();
    void panelLeftSlow();
    void panelRightFast();
    void panelRightSlow();

private:
    QLabel labelTitle;
    QLabel labelPositionInit;
    QLabel labelJob;
    QLabel labelPower;
    QLabel labelStatus;
    QLabel labelManual;
    QLabel labelPositioning;
    QPushButton buttonOn;
    QPushButton buttonOff;
    QPushButton buttonTrack;
    QPushButton buttonStop;
    QPushButton buttonLeftFast;
    QPushButton buttonLeftSlow;
    QPushButton buttonRightFast;
    QPushButton buttonRightSlow;
    QPushButton buttonPositionInit;
    QPushButton buttonPositionManual;
    QDoubleSpinBox valPositionInit;
    QDoubleSpinBox valPositionManual;
    QDoubleSpinBox valPositionCurrent;
    QLineEdit schedule;
    QwtDial dial;

    QVBoxLayout left;
    QVBoxLayout right;
    QHBoxLayout all;

};

#endif // SERSPANEL_H
