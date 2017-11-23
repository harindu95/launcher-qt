#include "pluginworker.h"
#include "easylogging++.h"
#include "mainwindow.h"
#include <algorithm>
#include <iostream>
#include "util.h"

extern std::vector<std::shared_ptr<Plugin>> plugins;
PluginWorker::PluginWorker(QObject *parent) : QObject(parent) {}


std::vector<Proposal> PluginWorker::query(QString txt) {

  std::vector<Proposal> data;
  for (auto plugin : plugins) {
    concat(data, plugin->query(txt));
  }

  return data;
}

void PluginWorker::run(QString text) { emit workFinished(query(text)); }
