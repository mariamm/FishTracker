#pragma once

#include <QMainWindow>
#include <QString>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QListWidget>

#include "Common.h"
#include "Tracker.h"
#include "Network.h"
#include "VideoPlayer.h"

#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    private:
        bool m_trackerMode;
        Server *m_trackerServer = nullptr;
        Client *m_visualizerClient = nullptr;

        BasicTracker *m_mockTracker = nullptr;
        VideoPlayer *m_videoPlayer = nullptr;
        cv::VideoCapture m_video;

        QString m_trackingOutputPath = "";

        //Help Menu
        QMenu *m_helpMenu= nullptr;;
        QAction *m_aboutAction= nullptr;;

        //Ui
        QListWidget *m_trackingOutputList= nullptr;;

        //Init functions
        void registerMetaTypes();
        void initAsTracker();
        void initAsVisualizer();

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:

        //tracker
        void onNewTrackingOutput(QString o);
        void onNewTrackedFrame(cv::Mat pixmap);
        void onStartTracker();
        void onStopTracker();
        void onPlayButtonPressed();

};
