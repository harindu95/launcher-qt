#include "mainwindow.h"

#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMainWindow>
#include <QTimer>

class QSingleApplication : public QApplication
{
    Q_OBJECT
public:
    void singleStart();

    QSingleApplication(int& argc, char* argv[]);

    ~QSingleApplication();

    bool _exit;

private:
    QLocalSocket* m_socket;
    QLocalServer* m_server;
    QLocalSocket* new_socket;
    bool new_socket_initialized = false;
    std::vector<std::string> args;


public slots:
    void connectToExistingApp();
    void startApplication(QLocalSocket::LocalSocketError);
    void getNewConnection();
    void readSocket();

signals:
    void messageReceived(std::string);
    void warning(std::string type, std::string msg);
    void exit();
};
