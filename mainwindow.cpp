#include "mainwindow.h"

#include "Plugins/qutebrowser.h"
#include "thirdparty/easylogging++.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>

std::vector<std::shared_ptr<Plugin>> plugins;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    //    ui->setupUi(this);
    center();
    plugins.push_back(std::make_shared<Applications>());
    plugins.push_back(std::make_shared<qutebrowser>());

    for(auto plugin : plugins)
    {
        plugin->setup();
    }
    initUi();
    thread->start();
    //    task = new PluginWorker();
    task.moveToThread(thread);

    qRegisterMetaType<std::vector<Proposal>>("std::vector<Proposal>");
    connect_task();
}

MainWindow::~MainWindow() {}

void MainWindow::connect_task()
{

    connect(this, SIGNAL(startWorker(QString, std::vector<Proposal>, QString)), &task,
            SLOT(run(QString, std::vector<Proposal>, QString)));
    connect(&task, SIGNAL(refresh(const std::vector<Proposal>&)), this,
            SLOT(updateProposals(const std::vector<Proposal>&)));
}


void MainWindow::focusOut()
{
    std::cout << "FOCUS OUT" << std::endl;
    searchString = "";
    searchBox->setText("");

    //    textEditFinished();
    this->hide();
}

void MainWindow::textEditFinished()
{
    visibleStart = 0;
    emit startWorker(searchString, results, previous_search);
}

void MainWindow::textChangedSlot(QString txt)
{
    LOG(DEBUG) << "text changed " << txt.toStdString();
    selected = 0;
    searchString = txt;
    //    timer->start(5);
    textEditFinished();
}

void MainWindow::goDown(int selected, int num = 1)
{
    this->selected = std::min<int>(results.size() - 1, selected + num);
    if(selected >= (visibleStart + 4))
    {
        visibleStart++;
        scroll();
    }
    else
    {
        LOG(DEBUG) << "Selected: " << selected << " results-size: " << results.size() << std::endl;
        setSelected();
    }
}
void MainWindow::goUp(int selected, int num = 1)
{
    this->selected = std::max<int>(0, selected - num);
    if(selected <= visibleStart)
    {
        visibleStart = std::max<int>(0, --visibleStart);
        scroll();
    }
    else
        setSelected();
}
void MainWindow::closeEvent(QCloseEvent* evnt) { thread->exit(0); }

void MainWindow::setSelected()
{
    unsigned int i = 0;
    for(auto item : items)
    {
        if(results.size() > i)
        {
            item->selectItem(selected == (i + visibleStart));
        }

        i++;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // esacpe
    if(event->key() == 0x01000000)
        hide();
    //#Up key
    else if(event->key() == 0x01000013)
    {
        goUp(selected, 1);
        //
    }
    //#Down key
    else if(event->key() == 0x01000015)
    {
        goDown(selected, 1);
        // self
        //                                                     .goDown()
    }
    else if(event->key() == Qt::Key_PageDown)
    {
        for(int i = 0; i < 5; i++) goDown(selected, 1);
        // self
        //                                                     .goDown()
    }
    else if(event->key() == Qt::Key_PageUp)
    {
        for(int i = 0; i < 5; i++) goUp(selected, 1);
        // self
        //                                                     .goDown()
    }


    //#enter key
    else if(event->key() == 0x01000004)
    {
        if(event->matches(QKeySequence::InsertLineSeparator))
            items[selected - visibleStart]->execute(true);
        else
            items[selected - visibleStart]->execute();

        focusOut();
    }
}


void MainWindow::initUi()
{

    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);
    thread = new QThread(this);
    //    timer = new QTimer(this);
    //    timer->setSingleShot(true);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(textEditFinished()));
    this->setCentralWidget(centralWidget);

    searchBox = new QLineEdit(this);
    QObject::connect(searchBox, SIGNAL(editingFinished()), this, SLOT(focusOut()));
    QObject::connect(searchBox, SIGNAL(textEdited(QString)), this, SLOT(textChangedSlot(QString)));

    searchBox->setStyleSheet("margin:0px;font-size:36px;font-style:normal;font-family:DejaVu "
                             "Sans;font-weight: 200;border:none;");
    //    setStyleSheet("*{ border:1px solid black;padding:0px;margin:0px;}");
    setStyleSheet("QMainWindow{ background:white;padding:0;margin:0;}");
    layout->addWidget(searchBox);
    layout->setSpacing(0);
    //    centralWidget->setLayout(layout);
    searchBox->setMinimumSize(QSize(630, 50));
    searchBox->setMaximumSize(QSize(630, 50));
    this->setFocusPolicy(Qt::StrongFocus);
    adjustSize();

    for(int i = 0; i < 5; i++)
    {
        auto item = new ResultWidget(this, plugins);
        layout->addWidget(item);
        items.push_back(item);
        item->hide();
    }
}


void MainWindow::center()
{
    this->move(QApplication::desktop()->screenGeometry().center() - this->rect().center());
}


void MainWindow::updateProposals(const std::vector<Proposal>& results)
{
    this->results = results;
    updateUi();
}
void MainWindow::scroll()
{
    LOG(DEBUG) << "SCROLL UI: " << results.size() << " " << searchString.toStdString();

    int i = 0;

    for(auto item : items)
    {
        if(results.size() > i)
        {
            item->changeItem(results.at(i + visibleStart), selected == (i + visibleStart));
            //            item->show();
        }

        i++;
    }
}
void MainWindow::updateUi()
{
    LOG(DEBUG) << "UPDATE UI: " << results.size() << " " << searchString.toStdString();

    int i = 0;

    for(auto item : items)
    {
        if(results.size() > i)
        {
            item->changeItem(results.at(i + visibleStart), selected == (i + visibleStart));
            item->show();
        }
        else
        {
            item->hide();
        }
        i++;
    }

    previous_search = searchString;
    QMetaObject::invokeMethod(this, "adjustSize", Qt::QueuedConnection);
}

void MainWindow::message(std::string msg)
{
    LOG(DEBUG) << "MESSAGE RECIEVED";
    this->show();
}
