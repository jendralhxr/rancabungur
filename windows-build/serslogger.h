#ifndef SERSLOGGER_H
#define SERSLOGGER_H

#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class SERSlogger : public QThread
{
    Q_OBJECT
public:
    explicit SERSlogger(QThread *parent = 0);
    SERSlogger(const char *filename);

signals:

public slots:
    void newAzimuth(double pos);
    void newElevation(double pos);
    void statusBusy();
    void statusStandby();

private:
    double latestAzimuth, latestElevation;
    QFile fileLog;
    QString string;
    QTextStream stream;
    void run();
    bool busy;

};

#endif // SERSLOGGER_H
