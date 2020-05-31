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
    registerMetaTypes();

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

void MainWindow::registerMetaTypes()
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QPixmap>("qpixmap");
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

    m_videoPlayer = new VideoPlayer(this);

    //connect(m_videoPlayer, &Player::playClicked, this, &MainWindow::onPlayButtonPressed);

    connect(m_videoPlayer, &VideoPlayer::play, this, &MainWindow::onPlayButtonPressed);
    connect(m_videoPlayer, &VideoPlayer::stop, this, &MainWindow::onStopTracker);

    mainLayout->addWidget(m_videoPlayer);
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
        m_video.release();
    }
    event->accept();
}

void MainWindow::onNewTrackingOutput(QString o)
{
    if(m_trackerServer != NULL)
    {
        m_trackerServer->sendMessage(o);
    }

    if(m_trackingOutputPath != "")
    {
        QFile f(m_trackingOutputPath);

        bool ok = f.open(QIODevice::WriteOnly | QIODevice::Append);

        if(!ok)
        {
          qDebug() << "Open file error!";
          return;
        }

        QTextStream stream(&f);
        stream << o<<"\n";
        f.close();
    }
    m_trackingOutputList->addItem(o);
    m_trackingOutputList->scrollToBottom();
}

void MainWindow::onNewTrackedFrame(cv::Mat frame)
{
    QPixmap pixmap = Common::cvMatToQPixmap(frame);
    m_videoPlayer->setFrame(pixmap);
    qApp->processEvents();
}

void MainWindow::onStartTracker()
{
    m_mockTracker = new Tracker();
    connect(m_mockTracker, &Tracker::output, this, &MainWindow::onNewTrackingOutput);
    connect(m_mockTracker, &Tracker::trackedFrame, this, &MainWindow::onNewTrackedFrame);

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
    else
    {
        m_trackingOutputPath = QFileDialog::getSaveFileName(this, tr("Save File"));
    }
}

void MainWindow::onStopTracker()
{
    if(m_mockTracker != nullptr)
        m_mockTracker->stop();
    else
    {
        onStartTracker();
    }

    if(m_video.isOpened())
    {
        m_video.release();
        return;
    }

}

void MainWindow::onPlayButtonPressed()
{
    if(m_mockTracker != nullptr)
        m_mockTracker->start();

    //stop video if already open
    if(m_video.isOpened())
    {
        m_video.release();
        return;
    }

    if(!m_video.open(m_videoPlayer->videoPath().toStdString()))
    {
        QMessageBox::critical(this, "Video Error", "Make sure you entered a correct and supported video file path.");
        qDebug()<<"Invalid video error";
        return;
    }

    cv::Mat frame;
    while(m_video.isOpened())
    {
        m_video >> frame;
        if(!frame.empty())
        {
            if(m_mockTracker)
                m_mockTracker->processFrame(frame);
            else
                onNewTrackedFrame(frame);
        }
        qApp->processEvents();
    }
}


