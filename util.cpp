#include "util.h"


std::string ascii_filter(QString h) {

  QString result;

  for (int i = 0; i < h.size(); ++i) {
    QChar qc = h.at(i);
    unsigned char c = *(unsigned char *)(&qc);
    if (c >= 127) {
      result.append("?");
    } else if (QChar(c).isPrint()) {
      result.append(QChar(c));
    }
  }

  return result.toStdString();
}
bool is_file_exist(const std::string fileName) {
  std::ifstream infile(fileName);
  return infile.good();
}
