#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Plugins/applications.h"
#include "pluginworker.h"
#include "resultwidget.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

namespace Ui
{
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_INVOKABLE void adjustSize() { QWidget::adjustSize(); }

public:
    explicit MainWindow(QWidget* parent = 0);

    std::vector<ResultWidget*> items;

    void keyPressEvent(QKeyEvent* event);

    void connect_task();

    void goDown(int selected, int num);

    void goUp(int selected, int num);

    void setSelected();

    void updateUi();
    void scroll();

    void showApp();

    ~MainWindow();


    void closeEvent(QCloseEvent* evnt);

private:
    void center();

    void initUi();


    QString searchString;
    QVBoxLayout* layout;
    QWidget* centralWidget;
    QLineEdit* searchBox;
    QThread* thread;
    QTimer* timer;
    PluginWorker task;

    // STATE
    int selected = 0;
    int visibleStart = 0;
    std::vector<Proposal> results;
    QString previous_search;

public slots:

    void focusOut();

    void textEditFinished();

    void textChangedSlot(QString txt);

    void message(std::string msg);
    void updateProposals(const std::vector<Proposal>& results);


signals:

    void startWorker(QString txt);
};
#endif // MAINWINDOW_H
