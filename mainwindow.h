#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "pluginworker.h"
#include "resultwidget.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

class MainWindow : public QWidget
{
    Q_OBJECT

    Q_INVOKABLE void adjustSize() { QWidget::adjustSize(); }

public:
    explicit MainWindow(QWidget* parent = 0);

    std::vector<ResultWidget*> items;

    void keyPressEvent(QKeyEvent* event);

    void connect_task();

    void goDown();

    void goUp();

    void updateUi();

    void showApp();

    ~MainWindow();


    void closeEvent(QCloseEvent* evnt);

private:
    void center();

    void initUi();


    QString searchString;
    QVBoxLayout* layout;
    QLineEdit* searchBox;
    QThread* thread;
    PluginWorker task;

    // STATE
    int selected = 0;
    int visibleStart = 0;
    std::vector<Proposal> results;

public slots:

    void focusOut();
    void textChangedSlot(QString txt);
    void message(std::string msg);
    void updateProposals(const std::vector<Proposal>& results);


signals:
    void startWorker(QString txt);
};
#endif // MAINWINDOW_H
