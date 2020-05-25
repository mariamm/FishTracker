#pragma once

#include <QObject>
#include <QString>
#include <QWidget>
#include <QLabel>
#include <QAbstractButton>
#include <QPixmap>
#include <QMediaPlayer>

class VideoPlayer : public QWidget
{
    Q_OBJECT
    public:
        VideoPlayer(QWidget *parent);
        void setFrame(QPixmap frame);
        QMediaPlayer::State state() const;

        QString videoPath() const;

    private:
        QLabel *m_videoLabel = nullptr;
        QAbstractButton *m_playButton = nullptr;
        QAbstractButton *m_stopButton = nullptr;

        QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;

        QString m_videoPath;
        QLabel *m_videoNameLabel = nullptr;

    public slots:
        void setState(QMediaPlayer::State state);

    private slots:
        void playClicked();
        void openVideoFile();

    signals:
        void play();
        void pause();
        void stop();
};
