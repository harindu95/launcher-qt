#include "qutebrowser.h"
// #include "../thirdparty/easylogging++.h"
#include <QDir>

qutebrowser::qutebrowser() {
  setup();
}


Proposal qutebrowser::parseLine(std::string line) {
    Proposal site;
    int i,j;
    if (line.find("h") != std::string::npos && line.find(" ", line.find("h")) != std::string::npos) {
            i =line.find("h");
            j = line.find(" ", i);
            site.comment = line.substr(i-1,j);
            site.name = line.substr(j).c_str();


    }else{
        return {};
    }
    site.type = "qutebrowser";
    site.icon = "web-browser";
    site.priority = 2;
    return site;
}


std::vector<Proposal> qutebrowser::extractData() {
    auto filename = QDir::homePath() + "/.local/share/qutebrowser/history";
    std::ifstream input(filename.toStdString());
    int i = 0;
    int c =0 ;
    for (std::string line; getline(input, line);) {
        c++;
    }
    std::ifstream inputFile(filename.toStdString());
    for (std::string line; getline(inputFile, line);) {
        if(i > c -500) {
            auto site = parseLine(line);
            if (!site.priority)
                continue;
            history.push_back(parseLine(line));
        }
        i++;

    }
    return history;
}

void qutebrowser::setup() {
    if (this->history.size() == 0)
        history = extractData();
}

std::vector<Proposal> qutebrowser::query(QString txt) {
    std::vector<Proposal> results;

    if (this->history.size() == 0)
        history = extractData();

    txt = txt.toLower();
    auto terms = txt.split(" ");
    std::vector<Proposal> data;
    if (txt.indexOf(previous_search) != -1 && !previous_search.isEmpty())
        data = previous_results;
    else
        data = history;
    auto regex = createRegex(txt);
    int i = 0;
    for (auto item : data) {
        if (regex.exactMatch(QString::fromStdString(item.comment + item.name))) {
            results.push_back(item);
            i++;

            if (i >= 200)
                break;
        }
    }
    // LOG(DEBUG) << "Querying inside qutebrowser-plugin";
    return results;
}

void qutebrowser::execute(Proposal item) {
    LOG(INFO) << "Executing:  .... " + item.comment + " & ";
    system(("qutebrowser " + item.comment + " &").c_str());
}

std::vector<Proposal> qutebrowser::get_proposals() {
    return history;
}
