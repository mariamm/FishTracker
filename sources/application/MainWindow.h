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
        //File Menu
        QMenu *m_fileMenu = nullptr;;
        QAction *m_openAction= nullptr;;
        QAction *m_saveAction= nullptr;;
        QAction *m_saveAsAction= nullptr;;
        QAction *m_exitAction= nullptr;;

        Server *m_trackerServer = nullptr;
        Client *m_visualizerClient = nullptr;

        Tracker *m_mockTracker = nullptr;
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

        void createActions();
        void createMenus();
        void updateActions();

        //File
        bool loadFile(const QString &);
        bool saveFile(const QString &fileName);

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:

        //tracker
        void onNewTrackingOutput(QString o);
        void onNewTrackedFrame(cv::Mat pixmap);
        void onStartTracker();
        void onStopTracker();
        void onPlayButtonPressed();

        //Help
        void about(){}
};
