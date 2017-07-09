#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QFocusEvent>
#include <QLineEdit>
class MyLineEdit : public QLineEdit
{


public:
    MyLineEdit(QWidget* parent = 0);
    //    void focusOutEvent(QFocusEvent* event);

    // signals:
    //    void focusOut(int nt);
};

#endif // MYLINEEDIT_H
