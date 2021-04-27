#include "Tracker.h"
#include <QDateTime>
#include <vector>
#include <QImage>
#include <QLabel>
#include <QApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "Common.h"

using namespace std;
using namespace cv;

BasicTracker::BasicTracker()
{
}
void BasicTracker::start()
{
    frameNumber = 0;
}

void BasicTracker::stop()
{
}

void BasicTracker::processFrame(cv::Mat frame)
{
    frameNumber++;

    SimpleBlobDetector::Params params;

    // Change thresholds
    //params.minThreshold = 10;
    //params.maxThreshold = 200;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 1500;

    // Filter by Circularity
    params.filterByCircularity = false;
    params.minCircularity = 0.1;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;


    // Storage for blobs
    vector<KeyPoint> keypoints;

    // Set up detector with params
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // Detect blobs
    detector->detect( frame, keypoints);

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
    // the size of the circle corresponds to the size of blob

    cv::Mat tracking;
    drawKeypoints( frame, keypoints, tracking, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    emit trackedFrame(tracking);
    jsonTrackingOutput(keypoints);
    qApp->processEvents();
}

void BasicTracker::jsonTrackingOutput(const vector<KeyPoint> &keypoints)
{
    //QDateTime t = QDateTime::currentDateTime();
    int id=0;

    QJsonObject json;
    QJsonArray jsonArray;
    for(KeyPoint p: keypoints)
    {
        QJsonObject object;
        object["id"] = id;
        object["x"] = p.pt.x;
        object["y"] = p.pt.y;
        object["framenumber"] = frameNumber;
        jsonArray.append(object);
        id++;
    }
    json["objects"] = jsonArray;
    QJsonDocument doc(json);
    QString o(doc.toJson(QJsonDocument::Compact));

    emit output(o);
}
