#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
class QLabel;
class QUdpSocket;
class QNetworkSession;
QT_END_NAMESPACE


class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server();
    void sendMessage(const QString &msg);
    void writeUdp(const QString& msg);

private slots:
    void initUdpSocket();
    void readPendingDatagrams();

private:
    QLabel *statusLabel = nullptr;
    QUdpSocket *m_udpSocket = nullptr;
    QHostAddress m_address;
    quint16 m_port;

};


#include <QDataStream>
#include <QDialog>
#include <QListWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
QT_END_NAMESPACE


class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void connectToServer();
    void readMessage();
    void displayError();
    void enableConnectButton();

    void visualizeInput(QString input);

private:
    QComboBox *hostCombo = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *connectButton = nullptr;
    QLabel *visualizer = nullptr;
    QUdpSocket* m_udpSocket = nullptr;
    QListWidget* m_trackingInput;
};
