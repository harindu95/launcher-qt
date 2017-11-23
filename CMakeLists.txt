cmake_minimum_required(VERSION 3.7)
project(launcher-qt)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -fsanitize=address  -D_GLIBCXX_DEBUG  ")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) 
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pg -pthread")
set(CMAKE_CXX_FLAGS_RELEASE  "-O2")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
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
  proposal.h
  util.h
  util.cpp
  )

add_subdirectory(Plugins)
add_subdirectory(lib)


find_package(Qt5 5.5.0 REQUIRED COMPONENTS
  Widgets
  Core
  Gui
  Network
  X11Extras)

set(QT_LIBS
  Qt5::Core
  Qt5::Network
  Qt5::X11Extras
  Qt5::Gui
  Qt5::Widgets)

find_package(X11 REQUIRED)
find_package(xcb REQUIRED)

add_executable(launcher-qt ${SOURCE_FILES})
target_link_libraries(launcher-qt
  globalshortcut
  ${QT_LIBS}
  X11
  xcb
  easylogging++)


add_custom_target(run
  COMMAND ${CMAKE_PROJECT_NAME}
  DEPENDS ${CMAKE_PROJECT_NAME}  
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
