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

private:
    double latestAzimuth, latestElevation;
    QFile fileLog;
    QString string;
    QTextStream stream;
    QDateTime current_time;
    void run();

};

#endif // SERSLOGGER_H
