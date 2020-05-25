#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

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
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << msg;

    if(m_clientConnection != nullptr)
    {
        m_clientConnection->write(block);
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

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(hostCombo, &QComboBox::editTextChanged, this, &Client::enableConnectButton);
    connect(connectButton, &QAbstractButton::clicked, this, &Client::connectToServer);

    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readMessage);

    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::displayError);

    QGridLayout *mainLayout = nullptr;
    mainLayout = new QGridLayout(this);
    m_trackingInput = new QListWidget(this);

    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(statusLabel, 1, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 2, 0, 1, 2);
    mainLayout->addWidget(m_trackingInput, 3, 0, 5, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered)
        {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        connectButton->setEnabled(true);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
}

void Client::connectToServer()
{
    connectButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(), 56253);
}

void Client::readMessage()
{
    in.startTransaction();

    QString nextInput;
    in >> nextInput;

    if (!in.commitTransaction())
        return;

    m_trackingInput->addItem(nextInput);
    m_trackingInput->scrollToBottom();
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
    connectButton->setEnabled((!networkSession || networkSession->isOpen()) && !hostCombo->currentText().isEmpty());
}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    enableConnectButton();
}
