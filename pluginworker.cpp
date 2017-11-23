#include "pluginworker.h"
#include "easylogging++.h"
#include "mainwindow.h"
#include <algorithm>
#include <iostream>

extern std::vector<std::shared_ptr<Plugin>> plugins;
PluginWorker::PluginWorker(QObject *parent) : QObject(parent) {}

template <typename InputIterator, typename T>
void copy(InputIterator it_begin, InputIterator it_end,
          std::vector<T> &output_vector) {

  for (auto i = it_begin; i != it_end; i++) {
    output_vector.push_back(*i);
  }
}

void concat(std::vector<Proposal> &vec1, std::vector<Proposal> vec2) {

  vec1.reserve(vec1.size() + vec2.size());
  vec1.insert(vec1.end(), vec2.begin(), vec2.end());
}

std::vector<Proposal> PluginWorker::query(QString txt) {

  std::vector<Proposal> data;
  for (auto plugin : plugins) {
    concat(data, plugin->query(txt));
  }

  return data;
}

void PluginWorker::run(QString text) { emit workFinished(query(text)); }
