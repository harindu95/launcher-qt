#include "plugin.h"

std::vector<Proposal> Plugin::query(QString txt)
{
    return {};
}
void Plugin::execute(Proposal item) { return; }

void Plugin::setup() {}

QString Plugin::join(QString text1, QString text2) {

  QString output;
  for (auto c : text2) {
    output += c;
    output += text1;
  }
  return output;
}

std::vector<Proposal> Plugin::get_proposals(){
  return {};
}
QRegExp Plugin::createRegex( QString searchString, bool strict){
  searchString = searchString.toLower();
  auto terms = searchString.split(" ");
  QString pattern = ".*";
  for(auto term : terms)
    {
      pattern += Plugin::join("[^\\s]*", term);
      pattern += ".*\\s?";
    }
  pattern += ".*";
  return QRegExp{ pattern, Qt::CaseInsensitive };
}
