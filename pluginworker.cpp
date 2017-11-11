#include "pluginworker.h"
#include "mainwindow.h"
#include "easylogging++.h"
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

std::vector<Proposal> PluginWorker::sort( std::vector<Proposal> proposals, QString searchString ){

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
  return proposals;
}

void concat(std::vector<Proposal>& vec1, std::vector<Proposal> vec2){

  vec1.reserve(vec1.size() + vec2.size());
  vec1.insert(vec1.end(),vec2.begin(),vec2.end());
} 
std::vector<Proposal> PluginWorker::query(QString txt)
{

  std::vector<Proposal> data ;
  for(auto plugin: plugins){
    concat(data,plugin->query(txt));
  }
  // if (txt.indexOf(previous_search) != -1 && !previous_search.isEmpty()) {
  //   data = &previous_results;
  //   }

  //   std::vector<Proposal> results;

  //   auto regex = createRegex(txt);

  //   auto it = std::find_if(data->begin(), data->end(), [&regex](Proposal item) {
  //       return regex.exactMatch(QString::fromStdString(item.comment + item.name + item.icon + item.exec));
  //   });

    // results.resize(std::distance(it,data->end()));
    // std::copy(it,data->end(), results.begin());

    // results = sort(std::move(results),txt);


    LOG(DEBUG) << "Querying";

    return data;
}
void PluginWorker::run(QString text)
{
    LOG(INFO) << "PluginWorker::run";

    //    std::vector<Proposal> all_results{ 300 };



    //    if(all_results.size() > 100)
    //    {
    //
    //        all_results = std::vector<Proposal>(&all_results[0], &all_results[100]);
    //    }

    LOG(INFO) << "EMITTING REFRESH SIGNAL";
    emit refresh(query(text));
}
