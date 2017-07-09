#include "mainwindow.h"
#include "singleapplication.h"
#include "thirdparty/easylogging++.h"
#include <QApplication>

INITIALIZE_EASYLOGGINGPP
int main(int argc, char* argv[])
{
    SingleApplication a(argc, argv);
    MainWindow w;
//    QApplication app(argc,argv);
//    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    a.setApplicationName("Launcher-qt");
    //    a.setQuitOnLastWindowClosed(false);

    QObject::connect(&a, SIGNAL(messageReceived(std::string)), &w, SLOT(message(std::string)));
    a.singleStart();
    if(!a._exit)
    {
        w.show();
        w.resize(650, 60);
        a.exec();
    }
}
