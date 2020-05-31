#pragma once

#include <QObject>
#include <QString>

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE


class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server();
    void sendMessage(const QString &msg);
    bool m_clientConnected;
private slots:
    void sessionOpened();
    void onNewClientConnected();
    void onClientDisconnected();

private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;
    QTcpSocket * m_clientConnection;
};


#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QListWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE


class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void connectToServer();
    void readMessage();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableConnectButton();
    void sessionOpened();

    void visualizeInput(QString input);

private:
    QComboBox *hostCombo = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *connectButton = nullptr;
    QLabel *visualizer = nullptr;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QListWidget* m_trackingInput;

    QNetworkSession *networkSession = nullptr;
};
