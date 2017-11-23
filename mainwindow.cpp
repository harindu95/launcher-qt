#include "mainwindow.h"
#include "Plugins/applications.h"
#include "Plugins/qutebrowser.h"
#include "easylogging++.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QProcess>

std::vector<std::shared_ptr<Plugin>> plugins;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  center();
  plugins.push_back(std::make_shared<Applications>());
  plugins.push_back(std::make_shared<qutebrowser>());

  initUi();
  thread->start();
  task.moveToThread(thread);

  qRegisterMetaType<std::vector<Proposal>>("std::vector<Proposal>");
  connect_task();
}

MainWindow::~MainWindow() {}

void MainWindow::connect_task() {

  connect(this, &MainWindow::startWorker, &task, &PluginWorker::run);
  connect(&task, &PluginWorker::workFinished, this,
          &MainWindow::updateProposals);
}

void MainWindow::focusOut() {
  LOG(INFO) << "FOCUS OUT" << std::endl;
  // searchString = "";
  searchBox->setText("");

  //    textEditFinished();
  this->hide();
}


void MainWindow::textChangedSlot(QString txt) {
  searchString = txt;
  visibleStart = 0;
  selected = 0;
  emit startWorker(txt);
}

void MainWindow::goDown() {
  this->selected = std::min<int>(results.size() - 1, selected+1);
  if (selected > (visibleStart + 4)) {
    visibleStart++;
  }
  updateUi();
}
void MainWindow::goUp() {
  this->selected = std::max<int>(0, selected + 1);
  if (selected <= visibleStart) {
    visibleStart = std::max<int>(0, --visibleStart);
  }
  updateUi();
}
void MainWindow::closeEvent(QCloseEvent *evnt) { thread->exit(0); }


void runTerminalCmd(std::string cmd) {

  LOG(INFO) << "Terminal CMD Execute: " << cmd;

  QProcess::startDetached(cmd.c_str());
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  // esacpe
  if (event->key() == 0x01000000)
    hide();
  //#Up key
  else if (event->key() == 0x01000013) {
    goUp();
    //
  }
  //#Down key
  else if (event->key() == 0x01000015) {
    goDown();
    // self
    //                                                     .goDown()
  } else if (event->key() == Qt::Key_PageDown) {
    for (int i = 0; i < 5; i++)
      goDown();
    // self
    //                                                     .goDown()
  } else if (event->key() == Qt::Key_PageUp) {
    for (int i = 0; i < 5; i++)
      goUp();
    // self
    //                                                     .goDown()
  }

  else if (event->key() == 0x01000004 &&
           event->modifiers() == Qt::ShiftModifier) {
    runTerminalCmd(searchString.toStdString());
    focusOut();

  }
  //#enter key
  else if (event->key() == 0x01000004) {
    if (event->matches(QKeySequence::InsertLineSeparator))
      items[selected - visibleStart]->execute(true);
    else
      items[selected - visibleStart]->execute();

    focusOut();
  }
}

void MainWindow::initUi() {

  layout = new QVBoxLayout(this);
  thread = new QThread(this);
  searchBox = new QLineEdit(this);
  QObject::connect(searchBox, &QLineEdit::editingFinished, this,
                   &MainWindow::focusOut);
  QObject::connect(searchBox, &QLineEdit::textEdited, this,
                   &MainWindow::textChangedSlot);

  searchBox->setStyleSheet(
      "margin:0px;font-size:36px;font-style:normal;font-family:DejaVu "
      "Sans;font-weight: 200;border:none;");
  // setStyleSheet("*{ border:1px solid black;padding:0px;margin:0px;}");
  setObjectName("MainWindow");
  setStyleSheet("QWidget#MainWindow{ background:white;padding:0;margin:0;}");
  layout->addWidget(searchBox);
  layout->setSpacing(0);
  //    centralWidget->setLayout(layout);
  searchBox->setMinimumSize(QSize(630, 50));
  setMinimumSize(QSize(640, 70));
  searchBox->setMaximumSize(QSize(630, 50));
  this->setFocusPolicy(Qt::StrongFocus);
  // this->layout()->setContentsMargin(0,0,0,0);
  adjustSize();

  for (int i = 0; i < 5; i++) {
    auto item = new ResultWidget(this, plugins);
    layout->addWidget(item);
    items.push_back(item);
    item->hide();
  }
}

void MainWindow::center() {
  this->move(QApplication::desktop()->screenGeometry().center() -
             this->rect().center());
}

void MainWindow::updateProposals(const std::vector<Proposal> &results) {
  this->results = results;
  updateUi();
}

void MainWindow::updateUi() {
  int i = 0;

  for (auto item : items) {
    if (results.size() > i) {
      item->changeItem(results.at(i + visibleStart),
                       selected == (i + visibleStart));
      item->show();
    } else {
      item->hide();
    }
    i++;
  }

  QMetaObject::invokeMethod(this, "adjustSize", Qt::QueuedConnection);
}

void MainWindow::message(std::string msg) { this->show(); }

void MainWindow::showApp() {
  LOG(DEBUG) << "Global shortcut activated";
  this->show();
}
