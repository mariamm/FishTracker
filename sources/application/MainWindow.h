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
#include "player.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    private:
        bool m_trackerMode;
        //File Menu
        QMenu *m_fileMenu;
        QAction *m_openAction;
        QAction *m_saveAction;
        QAction *m_saveAsAction;
        QAction *m_exitAction;

        Tracker *m_mockTracker;//To do: make unique ptr
        Server *m_trackerServer;

        Client* m_visualizerClient; //to do

        //Help Menu
        QMenu *m_helpMenu;
        QAction *m_aboutAction;

        //Ui
        QListWidget *m_trackingOutput;

        void initAsTracker();
        void initAsVisualizer();

        void createActions();
        void createMenus();
        void updateActions();

        //File
        bool loadFile(const QString &);
        bool saveFile(const QString &fileName);

    private slots:

        //tracker
        void addOutput(QString o);
        void onStartTracker();
        void onStopTracker();

        //Help
        void about(){}
};
