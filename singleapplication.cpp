#include "singleapplication.h"
#include "thirdparty/easylogging++.h"
#include <QFile>
#include <QMessageBox>
#include <QTimer>

SingleApplication::SingleApplication(int& argc, char* argv[])
: QApplication(argc, argv), _exit(false)
{
    for(int i = 0; i < argc; i++)
    {
        args.push_back(std::string(argv[i]));
    }
    m_server = new QLocalServer();
    m_socket = new QLocalSocket();
}

void SingleApplication::singleStart()
{
    _exit = false;
    QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(connectToExistingApp()));
    QObject::connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this,
                     SLOT(startApplication(QLocalSocket::LocalSocketError)));

    m_socket->connectToServer(applicationName(), QIODevice::WriteOnly);
}

void SingleApplication::connectToExistingApp()
{

    if(args.size() > 1)
    {
        LOG(DEBUG) << m_socket->serverName().toStdString();
        LOG(DEBUG) << m_socket->write(args[1].c_str());
        LOG(DEBUG) << m_socket->flush();
        _exit = true;
        connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(quit()));
    }
    else
    {

        QMessageBox::warning(0, "Already running", "Application is already running");
        LOG(DEBUG) << "Application is already running";
        QTimer::singleShot(250, this, SLOT(quit()));
    }
}


void SingleApplication::startApplication(QLocalSocket::LocalSocketError)
{
    if(m_server->listen(applicationName()))
    {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(getNewConnection()));
        LOG(DEBUG) << "startApplication " << m_server->serverName().toStdString();
    }
    else
    {

        _exit = true;
        QMessageBox::critical(0, "Error", "Error listening to socket");

        LOG(DEBUG) << "Error listening to socket.";
        QFile socket("/tmp/" + applicationName());
        socket.remove();
    }
}

void SingleApplication::getNewConnection()
{
    this->new_socket = m_server->nextPendingConnection();
    LOG(DEBUG) << "Get pending connection";
    new_socket_initialized = true;
    connect(new_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}
void SingleApplication::readSocket()
{
    LOG(DEBUG) << "CHECK SOCKET";
    auto f = new_socket->readAll();
    LOG(DEBUG) << "READ MESSAGE FROM SOCKET";
    emit messageReceived(QString(f).toStdString());
}

SingleApplication::~SingleApplication()
{
    m_server->close();
    m_socket->disconnectFromServer();
    if(new_socket_initialized)
        new_socket->close();
}