#pragma once

#include <QTimer>
#include <QObject>
#include <QString>

class Tracker : public QObject
{
    Q_OBJECT
    public:
        Tracker();

        void start();
        void stop();

    private:
        QTimer *m_outputTimer;

    private slots:
        void track();

     signals:
        void output(QString o);
};
