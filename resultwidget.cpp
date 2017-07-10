#include "resultwidget.h"

#include <QIcon>
#include <QPixmapCache>
#include <iostream>

// static QPixmapCache icon_cache;
static std::map<std::string,QPixmap> icon_cache;
ResultWidget::ResultWidget(QWidget* parent, std::vector<std::shared_ptr<Plugin>> plugins, QString text)
: QWidget(parent)
{
    this->plugins = plugins;
    initUi(text);
}

void ResultWidget::initUi(QString text)
{
    layout = new QHBoxLayout(this);
    label1 = new QLabel(text, this);
    label2 = new QLabel(this);
    icon = new QLabel(this);
    icon->setStyleSheet("margin:0;padding:0;width:64px;");
    layout->addWidget(icon);
    auto widget = new QWidget(this);
    auto vbox = new QVBoxLayout(widget);
    vbox->addWidget(label1);
    vbox->addWidget(label2);
    vbox->setSpacing(0);
    vbox->setMargin(4);
    //    widget->setStyleSheet("padding:0px;margin:0px;background:yellow;");
    setMinimumSize(QSize(600, 50));
    setMaximumSize(QSize(600, 50));


    layout->addWidget(widget, 1);
    layout->setSpacing(0);
    //    layout->setMargin(0);
    label1->setStyleSheet(
    "font-weight:normal;font-style:normal;font-style:normal;font-family:DejaVu "
    "Sans;font-weight: 200;font-size:20px;padding:0;");
    label2->setStyleSheet(
    "font-weight:normal;font-style:normal;font-style:normal;font-family:DejaVu Sans;font-weight: "
    "100;font-size:12px;");
    //    setContentsMargins(-1, -1, -1, 0);
    //    setStyleSheet("background:#ffffff;padding:0px;margin:0px");
    adjustSize();
    QIcon::setThemeName("Paper");
    default_icon = QIcon::fromTheme("applications-system");
    //    icon_cache.setCacheLimit(1024);
    //    QIcon::setThemeName()
    //    connect(this, SIGNAL(mouseClicked(QMouseEvent*)), this, SLOT(execute()));
}

void ResultWidget::mousePressEvent(QMouseEvent* event) { execute(); }

void ResultWidget::changeItem(Proposal result, bool selected)
{
    this->result = result;
    auto icon = result.icon.c_str();
    auto text = result.name;
    QPixmap image;
    if(icon_cache.find(icon) ==icon_cache.end())
    {

        if(QString(icon).startsWith('/'))
            image = QPixmap(icon).scaled(QSize(48, 48), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        else
        {
            image =
            QIcon::fromTheme(icon, QIcon::fromTheme((result.icon + "-48x48").c_str(), default_icon)).pixmap(48);
            //                .scaled(QSize(48, 48), Qt::IgnoreAspectRatio,
            //                Qt::SmoothTransformation);
        }
        icon_cache[icon]= image;
    }else{
      image = icon_cache[icon];
    }

    if(!image.isNull())
    {
        this->icon->setPixmap(image);
    }
    label1->setText(text.c_str());
    label2->setText(result.comment.c_str());
    this->setMinimumSize(QSize(630, 60));
    this->setMaximumSize(QSize(630, 60));
    selectItem(selected);
}
void ResultWidget::selectItem(bool selected)
{
    if(selected)
        this->setStyleSheet("background-color:#eeeeee;");
    else
        this->setStyleSheet("background-color:#ffffff");
}


void ResultWidget::execute(bool shift)
{
    std::cout << "Press" << std::endl;
    if(result.type == "applications")
    {
        std::cout << result.icon << std::endl;
        plugins.at(0)->execute(result);
    }
    else if(result.type == "qutebrowser")
    {
        plugins.at(1)->execute(result);
    }
    else
        std::cout << result.type << std::endl;

    //    else if(result["Type"] == "terminal")
    //    {
    //                     import terminal terminal.execute(self.result, **kwargs)
    //                     elif self.result['Type'] ==
    //        "web" : import web web.execute(self.result) elif self.result['Type']
    //        ==
    //        "file" : import file file
    //                 .execute(self.result)

    //                 elif self.result['Type'] ==
    //        "actions" : import actions actions.execute(self.result) elif
    //        self.result['Type'] ==
    //        "firefox" : import firefox firefox.execute(self.result)
    //    }
}
