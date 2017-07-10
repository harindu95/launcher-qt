#include "pluginworker.h"
#include "mainwindow.h"
#include "thirdparty/easylogging++.h"
#include <algorithm>
#include <iostream>
//#include <range/v3/all.hpp>

extern std::vector<std::shared_ptr<Plugin>> plugins;
PluginWorker::PluginWorker(QObject* parent) : QObject(parent) {}

QString PluginWorker::join(QString text1,QString text2){

  QString output;
  for(auto c: text2){
    output += c;
    output += text1;
  }
  return output;
}
QRegExp PluginWorker::createRegex( QString searchString, bool strict=false ){
  searchString = searchString.toLower();
  auto terms = searchString.split(" ");
  QString pattern = ".*";
  for(auto term : terms)
    {
      pattern += PluginWorker::join("[^\\s]*", term);
      pattern += ".*\\s?";
    }
    pattern += ".*";
    return QRegExp{ pattern, Qt::CaseInsensitive };
}
template<typename InputIterator,typename T>
void copy(InputIterator it_begin, InputIterator it_end,std::vector<T>& output_vector){

  for(auto i=it_begin;i!=it_end;i++){
    output_vector.push_back(*i);
  }
}

void PluginWorker::sort( std::vector<Proposal> &proposals, QString searchString ){

  auto regex = createRegex(searchString);
  auto bound = std::partition(proposals.begin(), proposals.end(), [&regex](Proposal item) {
      return regex.exactMatch(QString::fromStdString(item.name));
    });

  auto priority_bound = std::partition(proposals.begin(), bound, [&regex](Proposal item) {
      return item.priority==0;
    });
 
  std::sort(priority_bound, bound,
            [](Proposal a, Proposal b) { return (a.name.length() < b.name.length()); });
  std::sort(bound, proposals.end(), [](Proposal a, Proposal b) { return a.priority < b.priority; });
}

std::vector<Proposal> PluginWorker::query(QString txt,
                            const std::vector<Proposal>& all_results,
                            const std::vector<Proposal>& previous_results,
                            QString previous_search = "")
{

  const std::vector<Proposal>* data = &all_results;
  if (txt.indexOf(previous_search) != -1 && !previous_search.isEmpty()) {
    data = &previous_results;
    }

    std::vector<Proposal> results;

    auto regex = createRegex(txt);

    auto it = std::find_if(data->begin(), data->end(), [&regex](Proposal item) {
        return regex.exactMatch(QString::fromStdString(item.comment + item.name + item.icon + item.exec));
    });

    results.resize(std::distance(it,data->end()));
    std::copy(it,data->end(), results.begin());

    sort(results,txt);


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
