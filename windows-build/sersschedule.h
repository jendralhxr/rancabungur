#ifndef SERSSCHEDULE_H
#define SERSSCHEDULE_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDateTime>

class SERSschedule : public QThread
{
    Q_OBJECT
public:
    explicit SERSschedule(QThread *parent = 0);

signals:
    void execAzimuth(double pos);
    void execElevation(double pos);
    void showScheduledAzimuth(const char *sched);
    void showScheduledElevation(const char *sched);
    void trackingLineofSight();
    void trackingOutofsight();

// for clarity's sake
public slots:
    void jobTracking();
    void jobComplete();

private:
    void run();
    double scheduledAzimuth, scheduledElevation;
    QString string;
    QDateTime newyear;

};

#endif // SERSSCHEDULE_H