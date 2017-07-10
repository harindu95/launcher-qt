#include "mainwindow.h"
#include "singleapplication.h"
#include "thirdparty/easylogging++.h"
#include <QApplication>

INITIALIZE_EASYLOGGINGPP
int main(int argc, char* argv[])
{
    QSingleApplication a(argc, argv);
    MainWindow window;
//    QApplication app(argc,argv);
//    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    a.setApplicationName("Launcher-qt");
    //    a.setQuitOnLastWindowClosed(false);

    QObject::connect(&a, SIGNAL(messageReceived(std::string)), &window, SLOT(message(std::string)));
    a.singleStart();
    if(!a._exit)
    {
        window.show();
        window.resize(650, 60);
        a.exec();
    }
}
