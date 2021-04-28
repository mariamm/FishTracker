#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QPixmap>
#include <QPainter>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "Network.h"

#include <iostream>

Server::Server()
{
    m_clientConnected = false;
    m_clientConnection = nullptr;
    sessionOpened();
}

void Server::sessionOpened()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 56253))
    {
        QMessageBox::critical(NULL, tr("Tracking Server"), tr("Unable to start the server: %1.") .arg(tcpServer->errorString()));
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewClientConnected);
}

void Server::onNewClientConnected()
{
    m_clientConnected = true;
    m_clientConnection = tcpServer->nextPendingConnection();
    connect(m_clientConnection, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
}

void Server::onClientDisconnected()
{
    m_clientConnection->deleteLater();
    m_clientConnection = nullptr;
}

void Server::sendMessage(const QString &msg)
{
    QByteArray ba = msg.toUtf8();
    const char* c_str = ba.data();
    if(m_clientConnection != nullptr)
    {
        m_clientConnection->write(c_str);
    }
}

Client::Client(QWidget *parent)
    : QWidget(parent)
    , hostCombo(new QComboBox)
    , connectButton(new QPushButton(tr("Connect to Tracking Server")))
    , tcpSocket(new QTcpSocket(this))
{

    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty())
    {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));

    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    auto hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);

    statusLabel = new QLabel(tr("Tracking Server must be running."));

    connectButton->setDefault(true);
    connectButton->setEnabled(true);

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    
    connect(hostCombo, &QComboBox::editTextChanged, this, &Client::enableConnectButton);
    connect(connectButton, &QAbstractButton::clicked, this, &Client::connectToServer);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readMessage);
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &Client::displayError);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QGridLayout *grid = nullptr;
    grid = new QGridLayout;
    m_trackingInput = new QListWidget(this);

    grid->addWidget(hostLabel, 0, 0);
    grid->addWidget(hostCombo, 0, 1);
    grid->addWidget(statusLabel, 1, 0, 1, 2);
    grid->addWidget(buttonBox, 2, 0, 1, 2);
    grid->addWidget(m_trackingInput, 3, 0, 5, 2);

    mainLayout->addLayout(grid);

    visualizer = new QLabel(this);
    QPixmap p(400, 400);
    p.fill(Qt::black);
    visualizer->setPixmap(p);

    visualizer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(visualizer);
    setLayout(mainLayout);
    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Client::connectToServer()
{
    connectButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(), 56253);
}

void Client::readMessage()
{
    QByteArray ba = tcpSocket->readAll();
    QString nextInput(ba.data());

    m_trackingInput->addItem(nextInput);
    m_trackingInput->scrollToBottom();
    visualizeInput(nextInput);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("Client"),
                                     tr("The host was not found. Please check the host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Client"),
                                     tr("The connection was refused by the peer. Make sure the server is running, "
                                        "and check that the host name and port settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("Client"), tr("The following error occurred: %1.") .arg(tcpSocket->errorString()));
    }

    connectButton->setEnabled(true);
}

void Client::enableConnectButton()
{
    connectButton->setEnabled(!hostCombo->currentText().isEmpty());
}

void Client::visualizeInput(QString input)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(input.toUtf8());
    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object";
            return;
        }
    }
    else
    {
        qDebug() << "Invalid JSON.";
        return;
    }

    //video dimensions : 2704 × 1520
    QPixmap background(676, 380);
    background.fill(Qt::black);
    QJsonArray objectArray = obj.value("objects").toArray();
    int n = objectArray.count();

    for (int i = 0; i < n; i++)
    {
        QJsonObject o       = objectArray.at(i).toObject();
        int framenumber =   o["framenumber"].toInt();
        int id =            o["startPoint_y"].toInt();
        double x =          o["x"].toDouble();
        double y =          o["y"].toDouble();


        //downscale input values
        x = x/4.;
        y = y/4.;
        QPainter painter(&background);
        painter.setPen(Qt::red);
        painter.drawEllipse(QPointF(x,y), 15, 15);
        painter.end();
    }
    visualizer->setPixmap(background);
}
