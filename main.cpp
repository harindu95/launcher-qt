#include "mainwindow.h"
#include "singleapplication.h"
#include <QApplication>
#include "globalshortcut/hotkeymanager.h"
#include "easylogging++.h"

#ifdef INITIALIZE_EASYLOGGINGPP
INITIALIZE_EASYLOGGINGPP
#endif

int main(int argc, char* argv[])
{
    QSingleApplication a(argc, argv);
    MainWindow window;
    GlobalShortcut::HotkeyManager hotkeyManager;
    hotkeyManager.registerHotkey(Qt::ALT+Qt::Key_F3);
    QObject::connect(&hotkeyManager,&GlobalShortcut::HotkeyManager::hotKeyPressed,&window,&MainWindow::showApp);
//    QApplication app(argc,argv);
//    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    a.setApplicationName("Launcher-qt");
    //    a.setQuitOnLastWindowClosed(false);
    window.setWindowFlags(Qt::FramelessWindowHint);


    QObject::connect(&a, SIGNAL(messageReceived(std::string)), &window, SLOT(message(std::string)));
    a.singleStart();
    if(!a._exit)
    {
      if(argc > 2 && strcmp(argv[1],"show")==0 )
        window.show();
        window.resize(650, 60);
        a.exec();
    }
}
