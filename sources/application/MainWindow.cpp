#include <QtWidgets>
#include <QDockWidget>
#include <QSlider>
#include <QLabel>
#include <QPixmap>
#include <QLayout>
#include <QAbstractButton>

#include "MainWindow.h"

#include <opencv2/opencv.hpp>

MainWindow::MainWindow()
{
    setWindowTitle(tr("FishTracker v.1"));
    
    //Prompt to start software as tracker or visualizer
    QMessageBox msgBox;
    msgBox.setText("Software Mode");
    msgBox.setInformativeText("Do you want to start the software as a Tracker or Visualizer?");
    QAbstractButton* trackerButton = msgBox.addButton(tr("Tracker"), QMessageBox::YesRole);
    msgBox.addButton(tr("Visualizer"), QMessageBox::NoRole);

    msgBox.exec();

    //init
    if (msgBox.clickedButton()==trackerButton)
    {
        m_trackerMode = true;
        initAsTracker();
    }
    else
    {
        m_trackerMode = false;
        initAsVisualizer();
    }
}

void MainWindow::initAsTracker()
{
    QWidget *ui_area = new QWidget;
    setCentralWidget(ui_area);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *trackingLayout = new QVBoxLayout;
    m_trackingOutput = new QListWidget();

    QPushButton *startB = new QPushButton("Start");
    connect(startB, &QPushButton::clicked, this, &MainWindow::onStartTracker);
    QPushButton *stopB = new QPushButton("Stop");
    connect(stopB, &QPushButton::clicked, this, &MainWindow::onStopTracker);

    trackingLayout->addWidget(startB);
    trackingLayout->addWidget(stopB);
    trackingLayout->addWidget(m_trackingOutput);

    mainLayout->addLayout(trackingLayout);

    Player* player = new Player;
    mainLayout->addWidget(player);
    ui_area->setLayout(mainLayout);
}

void MainWindow::initAsVisualizer()
{
    /*m_visualizerClient = new Client(this);
    QWidget *ui_area = new QWidget;
    setCentralWidget(ui_area);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_visualizerClient);
    ui_area->setLayout(mainLayout);*/
}


void MainWindow::addOutput(QString o)
{
    if(m_trackerServer != nullptr)
    {
        m_trackerServer->sendMessage(o);
    }
    m_trackingOutput->addItem(o);
}

void MainWindow::onStartTracker()
{
    m_mockTracker = new Tracker();
    connect(m_mockTracker, &Tracker::output, this, &MainWindow::addOutput);

    //Prompt to start software as tracker or visualizer
    QMessageBox msgBox;
    msgBox.setText("Tracking output");
    msgBox.setInformativeText("Do you want to save or stream tracking output?");
    QAbstractButton* streamButton = msgBox.addButton(tr("Stream"), QMessageBox::YesRole);
    msgBox.addButton(tr("Save As..,"), QMessageBox::NoRole);
    msgBox.exec();

    //init
    if (msgBox.clickedButton()==streamButton)
    {
       //Start as a server
        m_trackerServer = new Server();
    }

    m_mockTracker->start();
}

void MainWindow::onStopTracker()
{

    /*ToDo:
     * stop tracking object
     * close output stream
     */

    if(m_mockTracker != nullptr)
        m_mockTracker->stop();
}


