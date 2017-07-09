#ifndef QUTEBROWSER_H
#define QUTEBROWSER_H

#include "plugin.h"

class qutebrowser : public Plugin {
public:
    qutebrowser();

    void execute(Proposal item) override;

    std::vector<Proposal> query(QString txt) override;

    std::vector<Proposal> extractData();

    Proposal parseLine(std::string line);

    std::vector<Proposal> history;

    void setup() override;
    std::vector<Proposal> get_proposals() override ;

private:
    std::vector<Proposal> previous_results;
    QString previous_search;

};

#endif // QUTEBROWSER_H
