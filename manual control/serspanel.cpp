#include "serspanel.h"

#define dialsize 290
#define max_accel 10
#define max_velo 200
#define max_pos 360

SERSpanel::SERSpanel(QWidget *parent) : QWidget(parent){

}

SERSpanel::SERSpanel(const char *nama, int origin, int value){
    title.setText(nama);
    buttonOn.setText("Motor On");
    buttonOff.setText("Motor Off");
    buttonExec.setText("Execute Postion");
    buttonStop.setText("Stop");
    buttonRightFast.setText("Right Fast");
    buttonRightSlow.setText("Right Slow");
    buttonLeftFast.setText("Left Fast");
    buttonLeftSlow.setText("Left Slow");

    labelAccel.setText("<b>Acceleration</b>");
    labelSpeed.setText("<b>Velocity</b>");
    labelPosition.setText("<b>Position</b>");
    labelJob.setText("<b>Job</b>");
    labelPower.setText("<b>Power</b>");
    labelStatus.setText("<b><font color=green size=5>Ready</font></b>");
    labelManual.setText("<b>Manual Drive</b>");
    labelFast.setText("<b>Fast Velocity</b>");
    labelSlow.setText("<b>Slow Velocity</b>");
    labelDrive.setText("<b><font color=green>Manual Drive</font></b>");
    labelPositioning.setText("<b><font color=green>Positioning</font></b>");
    labelAccelManual.setText("<b>Acceleration</b>");

    valPosition.setDecimals(4);
    valPosition.setMaximum(max_pos);
    valSpeed.setMaximum(max_velo);
    valAccel.setMaximum(max_accel);
    valAccelManual.setMaximum(max_accel);
    valSlow.setMaximum(max_velo);
    valFast.setMaximum(max_velo);

    dial.setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Ray,false,Qt::yellow));
    dial.setOrigin(origin);
    dial.setValue(value);
    dial.setMinimumSize(dialsize,dialsize);

    left.addWidget(&labelPower);
    left.addWidget(&buttonOn);
    left.addWidget(&buttonOff);
    left.addWidget(&labelJob);
    left.addWidget(&buttonExec);
    left.addWidget(&buttonStop);
    left.addWidget(&labelManual);
    left.addWidget(&buttonRightFast);
    left.addWidget(&buttonRightSlow);
    left.addWidget(&buttonLeftFast);
    left.addWidget(&buttonLeftSlow);

    mid.addWidget(&labelPositioning);
    mid.addWidget(&labelAccel);
    mid.addWidget(&valAccel);
    mid.addWidget(&labelSpeed);
    mid.addWidget(&valSpeed);
    mid.addWidget(&labelPosition);
    mid.addWidget(&valPosition);

    mid.addWidget(&labelDrive);
    mid.addWidget(&labelFast);
    mid.addWidget(&valFast);
    mid.addWidget(&labelSlow);
    mid.addWidget(&valSlow);
    mid.addWidget(&labelAccelManual);
    mid.addWidget(&valAccelManual);

    right.addWidget(&title);
    right.addWidget(&dial);
    right.addWidget(&labelStatus);

    all.addLayout(&left);
    all.addLayout(&mid);
    all.addLayout(&right);
    this->setLayout(&all);

    QObject::connect(&buttonOn,SIGNAL(clicked()),this,SLOT(panelOn()));
    QObject::connect(&buttonOff,SIGNAL(clicked()),this,SLOT(panelOff()));
    QObject::connect(&buttonExec,SIGNAL(clicked()),this,SLOT(panelExecPos()));
    QObject::connect(&buttonStop,SIGNAL(clicked()),this,SLOT(panelStop()));
    QObject::connect(&buttonRightFast,SIGNAL(clicked()),this,SLOT(panelRightFast()));
    QObject::connect(&buttonRightSlow,SIGNAL(clicked()),this,SLOT(panelRightSlow()));
    QObject::connect(&buttonLeftFast,SIGNAL(clicked()),this,SLOT(panelLeftFast()));
    QObject::connect(&buttonLeftSlow,SIGNAL(clicked()),this,SLOT(panelLeftSlow()));

}

void SERSpanel::updateDial(double value){
    dial.setValue(value);
}

void SERSpanel::statusReady(){
    labelStatus.setText("<b><font color=green size=5>Ready</font></b>");
}

void SERSpanel::statusBusy(){
    labelStatus.setText("<b><font color=red size=5>Busy</font></b>");
}

void SERSpanel::panelOn(){
    emit motorOn();
}

void SERSpanel::panelOff(){
    emit motorOff();
}

void SERSpanel::panelExecPos(){
    emit jobExec(valAccel.value(),valSpeed.value(),valPosition.value());
}

void SERSpanel::panelStop(){
    emit JobStop();
}

void SERSpanel::panelRightFast(){
    emit manualRightFast(valAccelManual.value(),valFast.value());
}

void SERSpanel::panelRightSlow(){
    emit manualRightSlow(valAccelManual.value(),valSlow.value());
}

void SERSpanel::panelLeftFast(){
    emit manualLeftFast(valAccelManual.value(),valFast.value());
}

void SERSpanel::panelLeftSlow(){
    emit manualLeftSlow(valAccelManual.value(),valSlow.value());
}

