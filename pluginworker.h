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
    std::vector<Proposal> query(QString txt);

signals:
    void workFinished(const std::vector<Proposal> &);

public slots:
    void run(QString text);
};


#endif // PLUGINWORKER_H
