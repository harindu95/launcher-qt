#include "applications.h"
#include "../util.h"
#include <QDir>
#include <QProcess>
#include <thread>

typedef std::string String;

Proposal Applications::parseFile(QString filename) {

  QFile input(filename);
  input.open(QIODevice::ReadOnly);
  Proposal desktopFile{};
  QString line;
  bool en_flag = true;
  while (!input.atEnd()) {
    line = input.readLine();
    if (line[0] == '#')
      continue;

    else if (line.indexOf("Name") != -1 && desktopFile.name == "") {

      desktopFile.name = line.mid(line.indexOf("=", line.indexOf("Name")) + 1)
                             .trimmed()
                             .toStdString();
      //            std::cout << line.mid(line.indexOf("=",
      //            line.indexOf("Name")) + 1).trimmed().toStdString() <<
      //                        " ... " << line.length() << std::endl;
    } else if (line.indexOf("Name[en_") != -1 && en_flag) {
      desktopFile.name =
          line.mid(line.indexOf("=", line.indexOf("Name")) + 1).toStdString();
      en_flag = false;
    } else if (line.indexOf("Comment") != -1 && desktopFile.comment == "") {
      desktopFile.comment =
          line.mid(line.indexOf("=", line.indexOf("Comment")) + 1)
              .trimmed()
              .toStdString();
    } else if (line.indexOf("Comment[en_") != -1) {
      desktopFile.comment =
          line.mid(line.indexOf("=", line.indexOf("Comment")) + 1)
              .trimmed()
              .toStdString();
    } else if (line.indexOf("Icon") != -1 && desktopFile.icon == "") {
      desktopFile.icon = line.mid(line.indexOf("=", line.indexOf("Icon")) + 1)
                             .trimmed()
                             .toStdString();
    } else if (line.indexOf("Exec") != -1 && desktopFile.exec == "") {

      desktopFile.exec = line.mid(line.indexOf("=", line.indexOf("Exec")) + 1)
                             .trimmed()
                             .remove(QRegExp("%."))
                             .toStdString();
    }
  }
  desktopFile.type = "applications";
  desktopFile.priority = 0;
  //    if(desktopFile.name == "qutebrowser")
  //        std::cout << desktopFile.icon << std::endl;
  return desktopFile;
}


std::vector<Proposal> Applications::getTerminalCommands() {
  std::vector<Proposal> commands;
  FILE *in;
  char buff[512];

  auto shell_cmd = "zsh -ci \'print -rl -- ${(ko)commands}\'";
  if (!(in = popen(shell_cmd, "r"))) {
    return {};
  }

  while (fgets(buff, sizeof(buff), in) != NULL) {
    std::string str(buff);
    str = str.substr(0, str.size() - 1);
    Proposal terminalCmd;
    terminalCmd.type = "applications";
    terminalCmd.icon = "binary";
    terminalCmd.priority = 1;
    terminalCmd.name = str;
    terminalCmd.comment = "";
    if (is_file_exist(QDir::homePath().toStdString() + "/bin/" + str)) {
      terminalCmd.exec = QDir::homePath().toStdString() + "/bin/" + str;
    } else if (QFile::exists(QDir::homePath() + QString("/.local/bin/") +
                             QString(str.c_str()))) {
      terminalCmd.exec = QDir::homePath().toStdString() + "/.local/bin/" + str;
    } else {
      terminalCmd.exec = str;
      //            std::cout << terminalCmd.exec << std::endl;
    }
    commands.push_back((terminalCmd));
  }
  pclose(in);
  return commands;
}

Applications::Applications() { setup(); }

std::vector<Proposal> Applications::extractData() {

  std::vector<Proposal> apps;
  QStringList nameFilter("*.desktop");
  QDir appDir("/usr/share/applications/");
  QDir homeDir(QDir::homePath() + "/.local/share/applications/");
  auto entries =
      appDir.entryInfoList(nameFilter) + homeDir.entryInfoList(nameFilter);
  for (auto entry = entries.begin(); entry != entries.end(); ++entry) {
    //        std::cout << (*entry).toStdString() << std::endl;
    apps.push_back(parseFile(entry->absoluteFilePath()));
  }
  auto terminal_cmds = getTerminalCommands();
  apps.insert(apps.end(), terminal_cmds.begin(), terminal_cmds.end());
  return apps;
}

void Applications::setup() {
  if (this->applications.size() == 0)
    applications = extractData();
}

std::vector<Proposal> Applications::query(QString txt) {

  std::vector<Proposal> results;

  if (this->applications.size() == 0)
    applications = extractData();

  if (txt.isEmpty())
    return applications;

  auto regex = createRegex(txt);
  std::vector<Proposal> data;
  if (txt.indexOf(previous_search) != -1 && !previous_search.isEmpty())
    data = previous_results;
  else
    data = applications;

  std::vector<Proposal> firsts;
  for (auto item : data) {
    String info = item.name + item.comment + item.exec + item.icon;
    QString q_info = QString(info.c_str()).toLower();
    if (regex.exactMatch(QString::fromStdString(info))) {
      if (regex.exactMatch(QString::fromStdString(item.name)))
        firsts.push_back(item);
      else {
        results.push_back(item);
      }
    }
  }
  // LOG(DEBUG) << "Querying inside applications-plugin";
  results.insert(results.begin(), firsts.begin(), firsts.end());
  previous_results = results;
  previous_search = txt;
  return results;
}

void Applications::execute(Proposal item) {

  LOG(INFO) << "Execute: " << item.exec;

  QProcess::startDetached((item.exec).c_str());
}

std::vector<Proposal> Applications::get_proposals() { return applications; }
