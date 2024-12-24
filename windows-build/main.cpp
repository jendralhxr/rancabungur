#include <QtGui/QApplication>
#include <serspanel.h>
#include <serscontrol.h>
#include <sersschedule.h>
#include <serslogger.h>
#include <QHBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SERSpanel *panelAzimuth = new SERSpanel("<font color=blue size=6>Azimuth</font>",270,360.0);
    SERSpanel *panelElevation = new SERSpanel("<font color=blue size=6>Elevation</font>",180,180.0);

    SERScontrol *controlAzimuth;
    SERScontrol *controlElevation;

    controlAzimuth = new SERScontrol("COM6",0);
    controlElevation = new SERScontrol("COM7",90);

//    if (argv[1]!=NULL) controlAzimuth = new SERScontrol(argv[1],0);
//    else controlAzimuth = new SERScontrol("COM6",0);
//    if (argv[2]!=NULL) controlElevation = new SERScontrol(argv[2],90);
//    controlElevation = new SERScontrol("COM7",90);

    SERSlogger *logger = new SERSlogger("log.txt"); // nicer if passed from args
    SERSschedule *scheduler = new SERSschedule();

    QObject::connect(panelAzimuth,SIGNAL(motorOn()),controlAzimuth,SLOT(motorOn()));
    QObject::connect(panelAzimuth,SIGNAL(motorOff()),controlAzimuth,SLOT(motorOff()));
    QObject::connect(panelAzimuth,SIGNAL(manualLeftFast()),controlAzimuth,SLOT(manualLeftFast()));
    QObject::connect(panelAzimuth,SIGNAL(manualLeftSlow()),controlAzimuth,SLOT(manualLeftSlow()));
    QObject::connect(panelAzimuth,SIGNAL(manualRightFast()),controlAzimuth,SLOT(manualRightFast()));
    QObject::connect(panelAzimuth,SIGNAL(manualRightSlow()),controlAzimuth,SLOT(manualRightSlow()));
    QObject::connect(panelAzimuth,SIGNAL(positionInit(double)),controlAzimuth,SLOT(positionInit(double)));
    QObject::connect(panelAzimuth,SIGNAL(positionExec(double)),controlAzimuth,SLOT(positionExec(double)));
    QObject::connect(panelAzimuth,SIGNAL(positionStop()),controlAzimuth,SLOT(jobStop()));
    QObject::connect(panelAzimuth,SIGNAL(positionStop()),scheduler,SLOT(jobComplete()));
    QObject::connect(panelAzimuth,SIGNAL(positionStop()),logger,SLOT(statusStandby()));
    QObject::connect(panelAzimuth,SIGNAL(positionTrack()),controlAzimuth,SLOT(jobTracking()));
    QObject::connect(panelAzimuth,SIGNAL(positionTrack()),scheduler,SLOT(jobTracking()));
    QObject::connect(panelAzimuth,SIGNAL(positionTrack()),logger,SLOT(statusBusy()));

    QObject::connect(panelElevation,SIGNAL(motorOn()),controlElevation,SLOT(motorOn()));
    QObject::connect(panelElevation,SIGNAL(motorOff()),controlElevation,SLOT(motorOff()));
    QObject::connect(panelElevation,SIGNAL(manualLeftFast()),controlElevation,SLOT(manualLeftFast()));
    QObject::connect(panelElevation,SIGNAL(manualLeftSlow()),controlElevation,SLOT(manualLeftSlow()));
    QObject::connect(panelElevation,SIGNAL(manualRightFast()),controlElevation,SLOT(manualRightFast()));
    QObject::connect(panelElevation,SIGNAL(manualRightSlow()),controlElevation,SLOT(manualRightSlow()));
    QObject::connect(panelElevation,SIGNAL(positionInit(double)),controlElevation,SLOT(positionInit(double)));
    QObject::connect(panelElevation,SIGNAL(positionExec(double)),controlElevation,SLOT(positionExec(double)));
    QObject::connect(panelElevation,SIGNAL(positionStop()),controlElevation,SLOT(jobStop()));
    QObject::connect(panelElevation,SIGNAL(positionStop()),scheduler,SLOT(jobComplete()));
    QObject::connect(panelElevation,SIGNAL(positionStop()),logger,SLOT(statusStandby()));
    QObject::connect(panelElevation,SIGNAL(positionTrack()),controlElevation,SLOT(jobTracking()));
    QObject::connect(panelElevation,SIGNAL(positionTrack()),scheduler,SLOT(jobTracking()));
    QObject::connect(panelElevation,SIGNAL(positionTrack()),logger,SLOT(statusBusy()));

    QObject::connect(controlAzimuth,SIGNAL(updatePosition(double)),panelAzimuth,SLOT(updateDial(double)));
    QObject::connect(controlAzimuth,SIGNAL(updatePosition(double)),logger,SLOT(newAzimuth(double)));
    QObject::connect(controlAzimuth,SIGNAL(positionComplete()),panelAzimuth,SLOT(statusStandby()));
    QObject::connect(controlAzimuth,SIGNAL(positionComplete()),logger,SLOT(statusStandby()));

    QObject::connect(controlElevation,SIGNAL(updatePosition(double)),panelElevation,SLOT(updateDial(double)));
    QObject::connect(controlElevation,SIGNAL(updatePosition(double)),logger,SLOT(newElevation(double)));
    QObject::connect(controlElevation,SIGNAL(positionComplete()),panelElevation,SLOT(statusStandby()));
    QObject::connect(controlElevation,SIGNAL(positionComplete()),logger,SLOT(statusStandby()));

    QObject::connect(scheduler,SIGNAL(execAzimuth(double)),controlAzimuth,SLOT(positionExec(double)));
    QObject::connect(scheduler,SIGNAL(showScheduledAzimuth(const char*)),panelAzimuth,SLOT(updateSchedule(const char*)));
    QObject::connect(scheduler,SIGNAL(execElevation(double)),controlElevation,SLOT(positionExec(double)));
    QObject::connect(scheduler,SIGNAL(showScheduledElevation(const char*)),panelElevation,SLOT(updateSchedule(const char*)));

    QObject::connect(scheduler,SIGNAL(trackingLineofSight()),panelAzimuth,SLOT(statusTrackingInsight()));
    QObject::connect(scheduler,SIGNAL(trackingLineofSight()),panelElevation,SLOT(statusTrackingInsight()));
    QObject::connect(scheduler,SIGNAL(trackingOutofsight()),panelAzimuth,SLOT(statusTrackingOutsight()));
    QObject::connect(scheduler,SIGNAL(trackingOutofsight()),panelElevation,SLOT(statusTrackingOutsight()));

    QHBoxLayout *layout =  new QHBoxLayout;
    layout->addWidget(panelAzimuth);
    layout->addWidget(panelElevation);

    QWidget *main =  new QWidget;
    main->setLayout(layout);
    main->show();

    return a.exec();
}