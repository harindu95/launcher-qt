#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H
#include "Plugins/plugin.h"
#include "proposal.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QWidget>
#include <map>
#include <memory>

typedef std::string String;

class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    ResultWidget(QWidget* parent, std::vector<std::shared_ptr<Plugin>> plugins, QString text = "");
    void mousePressEvent(QMouseEvent* event);
    void selectItem(bool selected);
    void changeItem(Proposal result, bool selected = false);

private:
    QHBoxLayout* layout;
    std::vector<std::shared_ptr<Plugin>> plugins;
    QLabel* label1;
    QLabel* label2;
    QLabel* icon;
    Proposal result;
    QIcon default_icon;
    void initUi(QString text = "");


signals:
    //    void mouseClicked(QMouseEvent* event);
public slots:
    void execute(bool shift = false);
};

#endif // RESULTWIDGET_H
