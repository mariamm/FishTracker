#pragma once

#include <QTimer>
#include <QObject>
#include <QString>
#include "VideoPlayer.h"
#include <QPixmap>
#include <vector>

#include <opencv2/opencv.hpp>

class BasicTracker : public QObject
{
    Q_OBJECT
    public:
        BasicTracker();
        void start();
        void stop();

        void processFrame(cv::Mat frame);
    private:
        int frameNumber = 0;

    private slots:
        void jsonTrackingOutput(const std::vector<cv::KeyPoint> &keypoints);

     signals:
        void output(QString o);
        void trackedFrame(cv::Mat frame);
};
