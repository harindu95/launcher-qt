#ifndef PLUGIN_H
#define PLUGIN_H
#include <QString>
#include <map>
#include <vector>
#include <QRegExp>
#include "../proposal.h"
#include "easylogging++.h"
typedef std::string String;
class Plugin
{

public:
    virtual std::vector<Proposal> query(QString txt);
    virtual void execute(Proposal item);
    virtual void setup();
    virtual std::vector<Proposal> get_proposals();

    QRegExp createRegex(QString txt, bool strict =false);
 private:
    QString join(QString str1, QString str2);
};

#endif // PLUGIN_H
