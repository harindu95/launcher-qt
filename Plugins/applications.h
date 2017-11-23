#ifndef APPLICATIONS_H
#define APPLICATIONS_H
#include "plugin.h"
#include <string>
#include <QString>
typedef std::string String;
class Applications : public Plugin
{
private:
    std::vector<Proposal> extractData();
    std::vector<Proposal> getTerminalCommands();
    Proposal parseFile(QString filename);
    //STATE
    QString previous_search;
    std::vector<Proposal> previous_results;
    std::vector<Proposal> applications;

public:
    Applications(); 
    std::vector<Proposal> get_proposals() override;
    /* virtual std::vector<Proposal> get_proposals() override ; */
    std::vector<Proposal> query(QString txt) override ;
    void execute(Proposal item) override ;
    void setup() override ;
};


#endif // APPLICATIONS_H
