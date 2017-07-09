#ifndef APPLICATIONS_H
#define APPLICATIONS_H
#include "plugin.h"
#include <QString>
#include <string>
typedef std::string String;
class Applications : public Plugin
{
private:
    std::vector<Proposal> extractData();
    std::vector<Proposal> getTerminalCommands();

    //STATE
    QString previous_search;
    std::vector<Proposal> previous_results;
    std::vector<Proposal> applications;

public:
    std::vector<Proposal> get_proposals() override ;
    std::vector<Proposal> query(QString txt) override ;
    void execute(Proposal item) override ;
    void setup() override ;
};


#endif // APPLICATIONS_H
