#include <QtWidgets>
#include <QDockWidget>
#include <QSlider>
#include <QLabel>
#include <QPixmap>
#include <QLayout>
#include <QAbstractButton>

#include "MainWindow.h"

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
    m_trackingOutputList = new QListWidget();

    QPushButton *startB = new QPushButton("Start");
    connect(startB, &QPushButton::clicked, this, &MainWindow::onStartTracker);
    QPushButton *stopB = new QPushButton("Stop");
    connect(stopB, &QPushButton::clicked, this, &MainWindow::onStopTracker);

    trackingLayout->addWidget(startB);
    trackingLayout->addWidget(stopB);
    trackingLayout->addWidget(m_trackingOutputList);

    mainLayout->addLayout(trackingLayout);

    Player* player = new Player;
    mainLayout->addWidget(player);
    ui_area->setLayout(mainLayout);
}

void MainWindow::initAsVisualizer()
{
    m_visualizerClient = new Client(this);
    QWidget *ui_area = new QWidget;
    setCentralWidget(ui_area);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_visualizerClient);
    ui_area->setLayout(mainLayout);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_video.isOpened())
    {
        QMessageBox::warning(this, "Warning", "Stop the video before closing the application!");
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::onNewTrackingOutput(QString o)
{
    if(m_trackerServer != NULL)
    {
        m_trackerServer->sendMessage(o);
    }

    if(trackingOutputPath != "")
    {
        QFile f(trackingOutputPath);
          bool ok = f.open(QIODevice::WriteOnly);

          if(!ok) {
              qDebug() << "open file error!";
              return;
          }

          QTextStream stream(&f);
          stream << o;
          f.close();
    }
    m_trackingOutputList->addItem(o);
    m_trackingOutputList->scrollToBottom();
}

void MainWindow::onStartTracker()
{
    m_mockTracker = new Tracker();
    connect(m_mockTracker, &Tracker::output, this, &MainWindow::onNewTrackingOutput);

    //Prompt to start software as tracker or visualizer
    QMessageBox msgBox;
    msgBox.setText("Tracking output");
    msgBox.setInformativeText("Do you want to save or stream tracking output?");
    QAbstractButton* streamButton = msgBox.addButton(tr("Stream"), QMessageBox::YesRole);
    msgBox.addButton(tr("Save As.."), QMessageBox::NoRole);
    msgBox.exec();

    //init
    if (msgBox.clickedButton()==streamButton)
    {
       //Start as a server
        if(m_trackerServer == nullptr)
            m_trackerServer = new Server();
    }

    m_mockTracker->start();
}

void MainWindow::onStopTracker()
{
    if(m_mockTracker != nullptr)
        m_mockTracker->stop();
}


