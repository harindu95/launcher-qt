#ifndef PLUGIN_H
#define PLUGIN_H
#include <QString>
#include <map>
#include <vector>
#include <QRegExp>
#include "../proposal.h"
typedef std::string String;
class Plugin
{

public:
    virtual std::vector<Proposal> query(QString txt);
    virtual void execute(Proposal item);
    virtual void setup();
    virtual std::vector<Proposal> get_proposals()=0;
    virtual ~Plugin() = 0;
};
std::string join(std::string text1, QString txt);

#endif // PLUGIN_H
