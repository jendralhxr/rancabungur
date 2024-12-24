#include "serspanel.h"
#include <math.h>

#define dialsize 300

SERSpanel::SERSpanel(QWidget *parent) : QWidget(parent){
}

SERSpanel::SERSpanel(const char *title, int origin, double max){
    labelTitle.setText(title);
    labelPositionInit.setText("<b>Initial Pos</b>");
    labelJob.setText("<b>Job</b>");
    labelPower.setText("<b>Power</b>");
    labelManual.setText("<b>Manual Drive</b>");
    labelPositioning.setText("<b>Positioning</b>");
    schedule.setText("--no schedule yet");
    buttonOn.setText("On");
    buttonOff.setText("Off");
    buttonTrack.setText("Track");
    buttonStop.setText("Stop");
    buttonPositionInit.setText("Set");
    buttonPositionManual.setText("Go");
    buttonLeftFast.setText("Left Fast");
    buttonLeftSlow.setText("Left Slow");
    buttonRightFast.setText("Right Fast");
    buttonRightSlow.setText("Right Slow");

    valPositionInit.setDecimals(4);
    valPositionInit.setMaximum(max);
    valPositionInit.setSingleStep(0.2);
    valPositionCurrent.setMaximum(max);
    valPositionCurrent.setDecimals(4);
    valPositionCurrent.setButtonSymbols(QDoubleSpinBox::NoButtons);
    valPositionManual.setMaximum(max);
    valPositionManual.setDecimals(4);
    dial.setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Ray,false,Qt::yellow));
    dial.setOrigin(origin);
    dial.setMinimumSize(dialsize,dialsize);
    dial.setScale(15,3,15); // for clarity's sake
//    dial.setReadOnly(true);

    left.addWidget(&labelPower);
    left.addWidget(&buttonOn);
    left.addWidget(&buttonOff);
    left.addWidget(&labelJob);
    left.addWidget(&buttonTrack);
    left.addWidget(&buttonStop);
    left.addWidget(&labelPositionInit);
    left.addWidget(&valPositionInit);
    left.addWidget(&buttonPositionInit);
    left.addWidget(&labelPositioning);
    left.addWidget(&valPositionManual);
    left.addWidget(&buttonPositionManual);
    left.addWidget(&labelManual);
    left.addWidget(&buttonLeftFast);
    left.addWidget(&buttonLeftSlow);
    left.addWidget(&buttonRightFast);
    left.addWidget(&buttonRightSlow);

    right.addWidget(&labelTitle);
    right.addWidget(&dial);
    right.addWidget(&valPositionCurrent);
    right.addWidget(&schedule);
    right.addWidget(&labelStatus);

    all.addLayout(&left);
    all.addLayout(&right);
    this->setLayout(&all);

    QObject::connect(&buttonOn,SIGNAL(clicked()),this,SLOT(panelOn()));
    QObject::connect(&buttonOff,SIGNAL(clicked()),this,SLOT(panelOff()));
    QObject::connect(&buttonTrack,SIGNAL(clicked()),this,SLOT(panelTrack()));
    QObject::connect(&buttonStop,SIGNAL(clicked()),this,SLOT(panelStop()));
    QObject::connect(&buttonPositionInit,SIGNAL(clicked()),this,SLOT(panelPositionInit()));
    QObject::connect(&buttonPositionManual,SIGNAL(clicked()),this,SLOT(panelPositionManual()));
    QObject::connect(&buttonLeftFast,SIGNAL(clicked()),this,SLOT(panelLeftFast()));
    QObject::connect(&buttonLeftSlow,SIGNAL(clicked()),this,SLOT(panelLeftSlow()));
    QObject::connect(&buttonRightFast,SIGNAL(clicked()),this,SLOT(panelRightFast()));
    QObject::connect(&buttonRightSlow,SIGNAL(clicked()),this,SLOT(panelRightSlow()));
    QObject::connect(&dial,SIGNAL(valueChanged(double)),&valPositionCurrent,SLOT(setValue(double)));
    //QObject::connect(&valPositionCurrent,SIGNAL(valueChanged(double)),&dial,SLOT(setValue(double)));
    statusStandby();
}

void SERSpanel::updateDial(double value){
    dial.setValue(fmod(value,360.0));
}

void SERSpanel::updateSchedule(const char *string){
    schedule.setText(string);
}

void SERSpanel::statusStandby(){
    labelStatus.setText("<font color=green size=6>Status: Standby</font>");
}

void SERSpanel::statusBusy(){
    labelStatus.setText("<font color=red size=6>Status: Busy</font>");
}

void SERSpanel::statusTrackingInsight(){
    labelStatus.setText("<font color=blue size=6>Status: Tracking (line-of-sight)</font>");
}

void SERSpanel::statusTrackingOutsight(){
    labelStatus.setText("<font color=orange size=6>Status: Tracking (out-of-sight)</font>");
}

void SERSpanel::panelOn(){
    emit motorOn();
}

void SERSpanel::panelOff(){
    emit motorOff();
}

void SERSpanel::panelStop(){
    emit positionStop();
}

void SERSpanel::panelPositionInit(){
    dial.setValue(valPositionInit.value());
    emit positionInit(valPositionInit.value());
}

void SERSpanel::panelLeftFast(){
    statusBusy();
    emit manualLeftFast();
}

void SERSpanel::panelLeftSlow(){
    statusBusy();
    emit manualLeftSlow();
}

void SERSpanel::panelRightFast(){
    statusBusy();
    emit manualRightFast();
}

void SERSpanel::panelRightSlow(){
    statusBusy();
    emit manualRightSlow();
}

void SERSpanel::panelPositionManual(){
    statusBusy();
    emit positionExec(valPositionManual.value());
}

void SERSpanel::panelTrack(){
    //statusTracking();
    emit positionTrack();
}
