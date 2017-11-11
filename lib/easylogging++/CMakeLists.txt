project(easylogging++)

set(
  SRC
  easylogging++.cc
  include/easylogging++.h  )

add_library(easylogging++ ${SRC})
target_include_directories(easylogging++
  PUBLIC
  include/
  )

