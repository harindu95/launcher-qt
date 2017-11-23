#ifndef UTIL_H
#define UTIL_H
#include <QString>
#include <fstream>
#include <string>
#include <vector>

template <typename InputIterator, typename T>
void copy(InputIterator it_begin, InputIterator it_end,
          std::vector<T> &output_vector) {

  for (auto i = it_begin; i != it_end; i++) {
    output_vector.push_back(*i);
  }
}

template <typename T> void concat(std::vector<T> &vec1, std::vector<T> vec2) {

  vec1.reserve(vec1.size() + vec2.size());
  vec1.insert(vec1.end(), vec2.begin(), vec2.end());
}

std::string ascii_filter(QString h);

bool is_file_exist(const std::string fileName);
#endif
