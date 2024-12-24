#include <QtGui/QApplication>
#include <serspanel.h>
#include <serscontrol.h>

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SERSpanel *azimuth = new SERSpanel("<b><font color=blue size=5>Azimuth</font></b>",270,0);
    SERSpanel *elevate = new SERSpanel("<b><font color=blue size=5>Elevation</font></b>",180,0);

    QHBoxLayout *form = new QHBoxLayout();
    form->addWidget(azimuth);
    form->addWidget(elevate);

    QWidget *view = new QWidget();
    view->setLayout(form);
    view->show();

    SERScontrol *con_azimuth = new SERScontrol("/dev/tty3","azimuthlog",0);
    SERScontrol *con_elevate = new SERScontrol("/dev/tty4","elevatelog",90);

    // konekkan objek kontrol dan objek panel

    QObject::connect(azimuth,SIGNAL(motorOn()),con_azimuth,SLOT(motorOn()));
    QObject::connect(azimuth,SIGNAL(motorOff()),con_azimuth,SLOT(motorOff()));
    QObject::connect(azimuth,SIGNAL(jobExec(int,int,double)),con_azimuth,SLOT(jobExec(int,int,double)));
    QObject::connect(azimuth,SIGNAL(JobStop()),con_azimuth,SLOT(jobStop()));
    QObject::connect(azimuth,SIGNAL(manualLeftFast(int,int)),con_azimuth,SLOT(manualLeftFast(int,int)));
    QObject::connect(azimuth,SIGNAL(manualLeftSlow(int,int)),con_azimuth,SLOT(manualLeftSlow(int,int)));
    QObject::connect(azimuth,SIGNAL(manualRightFast(int,int)),con_azimuth,SLOT(manualRightFast(int,int)));
    QObject::connect(azimuth,SIGNAL(manualRightSlow(int,int)),con_azimuth,SLOT(manualRightSlow(int,int)));

    QObject::connect(con_azimuth,SIGNAL(updatePostion(double)),azimuth,SLOT(updateDial(double)));
    QObject::connect(con_azimuth,SIGNAL(jobComplete()),azimuth,SLOT(statusReady()));
    QObject::connect(con_azimuth,SIGNAL(jobBusy()),azimuth,SLOT(statusBusy()));

    QObject::connect(elevate,SIGNAL(motorOn()),con_elevate,SLOT(motorOn()));
    QObject::connect(elevate,SIGNAL(motorOff()),con_elevate,SLOT(motorOff()));
    QObject::connect(elevate,SIGNAL(jobExec(int,int,double)),con_elevate,SLOT(jobExec(int,int,double)));
    QObject::connect(elevate,SIGNAL(JobStop()),con_elevate,SLOT(jobStop()));
    QObject::connect(elevate,SIGNAL(manualLeftFast(int,int)),con_elevate,SLOT(manualLeftFast(int,int)));
    QObject::connect(elevate,SIGNAL(manualLeftSlow(int,int)),con_elevate,SLOT(manualLeftSlow(int,int)));
    QObject::connect(elevate,SIGNAL(manualRightFast(int,int)),con_elevate,SLOT(manualRightFast(int,int)));
    QObject::connect(elevate,SIGNAL(manualRightSlow(int,int)),con_elevate,SLOT(manualRightSlow(int,int)));

    QObject::connect(con_elevate,SIGNAL(updatePostion(double)),elevate,SLOT(updateDial(double)));
    QObject::connect(con_elevate,SIGNAL(jobComplete()),elevate,SLOT(statusReady()));
    QObject::connect(con_elevate,SIGNAL(jobBusy()),elevate,SLOT(statusBusy()));

    con_azimuth->start(QThread::NormalPriority);
    con_elevate->start(QThread::NormalPriority);

    return a.exec();
}
