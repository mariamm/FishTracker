#pragma once

#include <QTimer>
#include <QObject>
#include <QString>
#include "VideoPlayer.h"
#include <QPixmap>
#include <vector>

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
        int frameNumber = 0;
    private slots:
        void jsonTrackingOutput(const std::vector<cv::KeyPoint> &keypoints);

     signals:
        void output(QString o);
        void trackedFrame(cv::Mat frame);
};
