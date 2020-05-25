#include "Tracker.h"
#include <QDateTime>

Tracker::Tracker()
{
        m_outputTimer = new QTimer(this);
        connect(m_outputTimer, &QTimer::timeout, this, &Tracker::track);
}
void Tracker::start()
{

    m_outputTimer->start(1000);
}

void Tracker::stop()
{
    m_outputTimer->stop();
}

void Tracker::track()
{
    QDateTime t = QDateTime::currentDateTime();
    QString o = QString("id:1_x:1_y:1_timestamp:0%1").arg(t.toString("hh:mm:ss.zzz"));
    emit output(o);
}
