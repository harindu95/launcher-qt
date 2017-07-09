#include "pluginworker.h"
#include "mainwindow.h"
#include "thirdparty/easylogging++.h"
#include <algorithm>
#include <iostream>
//#include <range/v3/all.hpp>

extern std::vector<std::shared_ptr<Plugin>> plugins;
PluginWorker::PluginWorker(QObject* parent) : QObject(parent) {}


std::vector<Proposal> query(QString txt,
                            const std::vector<Proposal>& all_results,
                            const std::vector<Proposal>& previous_results,
                            QString previous_search = "")
{

    txt = txt.toLower();
    auto terms = txt.split(" ");
    std::string pattern = ".*";
    for(auto term : terms)
    {
        pattern += join("[^\\s]*", term);
        pattern += ".*\\s?";
    }
    const std::vector<Proposal>* data;
    if(txt.indexOf(previous_search) != -1 && !previous_search.isEmpty())
    {
        data = &previous_results;
    }
    else
    {
        data = &all_results;
    }

    pattern += ".*";
    QRegExp regex{ QString::fromStdString(pattern), Qt::CaseInsensitive };

    std::vector<Proposal> results;


    auto it = std::find_if(data->begin(), data->end(), [&regex](Proposal item) {
        return regex.exactMatch(QString::fromStdString(item.comment + item.name + item.icon + item.exec));
    });

    //    std::vector<Proposal> results = *data | ranges::view::filter([&regex](Proposal item) {
    //        return regex.exactMatch(QString::fromStdString(item.comment + item.name + item.icon +
    //        item.exec));
    //    });
    //    auto bound = ranges::partition(results, [&regex](Proposal item) {
    //        return regex.exactMatch(QString::fromStdString(item.name));
    //    });


    while(it != data->end())
    {
        results.push_back(*it);
        it++;
    }

    auto bound = std::partition(results.begin(), results.end(), [&regex](Proposal item) {
        return regex.exactMatch(QString::fromStdString(item.name));
    });

    std::sort(results.begin(), bound,
              [](Proposal a, Proposal b) { return (a.priority < b.priority); });
    std::sort(bound, results.end(), [](Proposal a, Proposal b) { return a.priority < b.priority; });


    LOG(DEBUG) << "Querying";

    return results;
}
void PluginWorker::run(QString text, std::vector<Proposal> previous_results, QString previous_search)
{
    LOG(INFO) << "PluginWorker::run";

    //    std::vector<Proposal> all_results{ 300 };
    auto applications = plugins[0]->get_proposals();

    auto qutebrowser = plugins[1]->get_proposals();

    //    results.reserve(applications.size() + qutebrowser.size());
    auto all_results = applications;

    all_results.insert(all_results.end(), qutebrowser.begin(), qutebrowser.end());


    //    if(all_results.size() > 100)
    //    {
    //
    //        all_results = std::vector<Proposal>(&all_results[0], &all_results[100]);
    //    }

    LOG(INFO) << "EMITTING REFRESH SIGNAL";
    emit refresh(query(text, all_results, previous_results, previous_search));
}
