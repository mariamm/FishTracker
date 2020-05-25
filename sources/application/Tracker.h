#pragma once

#include <QTimer>
#include <QObject>
#include <QString>

#include <opencv2/opencv.hpp>

class Tracker : public QObject
{
    Q_OBJECT
    public:
        Tracker();

        void start();
        void stop();

        void processFrame(cv::Mat frame);
    private:
        QTimer *m_outputTimer;

    private slots:
        void track();

     signals:
        void output(QString o);
};
