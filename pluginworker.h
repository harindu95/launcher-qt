#ifndef PLUGINWORKER_H
#define PLUGINWORKER_H

#include <QObject>
#include <map>
#include <vector>
#include "proposal.h"

typedef std::string String;

class PluginWorker : public QObject
{
    Q_OBJECT
public:
    explicit PluginWorker(QObject* parent = 0);


signals:
    void workFinished();
    void refresh(const std::vector<Proposal> &);

public slots:
    void run(QString text, std::vector<Proposal> previous_results, QString previous_search);
};


#endif // PLUGINWORKER_H
