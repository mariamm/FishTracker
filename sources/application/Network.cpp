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
    initUdpSocket();
}

void Server::initUdpSocket()
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress::Any, 56253);

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &Server::readPendingDatagrams);
}

void Server::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        m_port = datagram.senderPort();
        m_address = datagram.senderAddress();
    }
}

void Server::sendMessage(const QString &msg)
{
    writeUdp(msg);
}

void Server::writeUdp(const QString& msg)
{
    QByteArray ba = msg.toUtf8();
    const char* c_str = ba.data();
    m_udpSocket->writeDatagram(c_str, ba.size(), m_address, m_port);
}

Client::Client(QWidget *parent)
    : QWidget(parent)
    , hostCombo(new QComboBox)
    , connectButton(new QPushButton(tr("Connect to Tracking Server")))
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
        if (!ipAddressesList.at(i).isGlobal())
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

    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress::Any, 56254);
    connect(m_udpSocket, &QUdpSocket::readyRead,
        this, &Client::readMessage);
}

void Client::connectToServer()
{
    int ret = m_udpSocket->writeDatagram("Hello", 5, QHostAddress(hostCombo->currentText()), 56253);
    if (ret == -1)
        displayError();
}

void Client::readMessage()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        QByteArray ba = datagram.data();
        QString nextInput(ba.data());

        m_trackingInput->addItem(nextInput);
        m_trackingInput->scrollToBottom();
        visualizeInput(nextInput);
    }
}

void Client::displayError()
{
    QMessageBox::information(this, tr("Client"), tr("Failed to connect!"));
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
        painter.drawEllipse(QPointF(x,y), 35, 35);
        painter.end();
    }
    visualizer->setPixmap(background);
}
