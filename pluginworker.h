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
    QString join(QString,QString);
    QRegExp createRegex(QString,bool strict);
    void sort(std::vector<Proposal>& proposals,QString searchString ); 
    std::vector<Proposal> query(QString txt,
      const std::vector<Proposal>& all_results,
      const std::vector<Proposal>& previous_results,
                                QString previous_search);

signals:
    void workFinished();
    void refresh(const std::vector<Proposal> &);

public slots:
    void run(QString text, std::vector<Proposal> previous_results, QString previous_search);
};


#endif // PLUGINWORKER_H
