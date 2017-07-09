cmake_minimum_required(VERSION 3.7)
project(launcher-qt)
#option(USE_CLANG "build application with clang" ON)
# set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -pthread   -fsanitize=undefined  -D_GLIBCXX_DEBUG  ")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) 
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pg -pthread")
set(CMAKE_CXX_FLAGS_RELEASE  "-O2")
set(SOURCE_FILES
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mylineedit.cpp
  mylineedit.h
  pluginworker.cpp
  pluginworker.h
  resultwidget.cpp
  resultwidget.h
  singleapplication.h
  singleapplication.cpp
  Plugins/applications.cpp
  Plugins/applications.h
  Plugins/plugin.cpp
  Plugins/plugin.h
  Plugins/qutebrowser.cpp
  Plugins/qutebrowser.h
  proposal.h)

INCLUDE_DIRECTORIES(thirdparty/include)
#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/include)

set(LOG_LIB thirdparty/easylogging++.cc thirdparty/easylogging++.h)
#qt_wrap_cpp(main_window_H mainwindow.h)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
#find_package(Qt4 REQUIRED QtGui QtCore QtNetwork)
set(CMAKE_AUTOMOC ON)
message(STATUS ${RANGE_LIB})
add_executable(launcher-qt ${SOURCE_FILES} ${LOG_LIB})
target_link_libraries(launcher-qt Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)

add_custom_target(run
  COMMAND ${CMAKE_PROJECT_NAME}
  DEPENDS ${CMAKE_PROJECT_NAME} 
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
