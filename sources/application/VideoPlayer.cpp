#include "VideoPlayer.h"

#include <QLayout>
#include <QToolButton>
#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QFileDialog>
#include <QPushButton>
#include <QStandardPaths>
#include <QMediaPlayer>

VideoPlayer::VideoPlayer(QWidget *parent) : QWidget(parent)
{
    m_videoLabel = new QLabel(this);
    m_videoNameLabel = new QLabel("no video", this);

    QVBoxLayout *mainLayout  = new QVBoxLayout;

    mainLayout->addWidget(m_videoNameLabel);
    mainLayout->addWidget(m_videoLabel);

    //Player controls
    m_playButton = new QToolButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked, this, &VideoPlayer::playClicked);

    m_stopButton = new QToolButton(this);
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    connect(m_stopButton, &QAbstractButton::clicked, this, &VideoPlayer::stop);
    QBoxLayout *mediaButtonsLayout = new QHBoxLayout;
    mediaButtonsLayout->setContentsMargins(0, 0, 0, 0);
    mediaButtonsLayout->addWidget(m_playButton);
    mediaButtonsLayout->addWidget(m_stopButton);

    QPushButton *openButton = new QPushButton(tr("Open"), this);

    connect(openButton, &QPushButton::clicked, this, &VideoPlayer::openVideoFile);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openButton);
    controlLayout->addLayout(mediaButtonsLayout);
    controlLayout->addStretch(1);

    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);
}

void VideoPlayer::setFrame(QPixmap frame)
{
    m_videoLabel->setPixmap(frame);
}

void VideoPlayer::playClicked()
{
    switch (m_playerState)
    {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            emit play();
            break;
        case QMediaPlayer::PlayingState:
            emit pause();
            break;
    }
}


QMediaPlayer::State VideoPlayer::state() const
{
    return m_playerState;
}

QString VideoPlayer::videoPath() const
{
    return m_videoPath;
}

void VideoPlayer::setState(QMediaPlayer::State state)
{
    if (state != m_playerState)
    {
        m_playerState = state;

        switch (state)
        {
        case QMediaPlayer::StoppedState:
            m_stopButton->setEnabled(false);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void VideoPlayer::openVideoFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open video"));
    QStringList supportedMimeTypes = QMediaPlayer::supportedMimeTypes();
    fileDialog.setMimeTypeFilters(supportedMimeTypes);

    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
    {
        m_videoPath = fileDialog.selectedFiles().first();
        QFileInfo fi(m_videoPath);
        m_videoNameLabel->setText(fi.fileName());
    }
}
