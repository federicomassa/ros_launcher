cmake_minimum_required(VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)                                                                                                                                                                                     

find_package(Qt5 COMPONENTS Core Qml Quick Widgets REQUIRED)
find_package(Boost COMPONENTS regex REQUIRED)


# Find includes in corresponding build directories                                                                                                                                                                                     
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed                                                                                                                                                                                  
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files                                                                                                                                                                                      
set(CMAKE_AUTOUIC ON)
# Manage resources                                                                                                                                                                                                                     
set(CMAKE_AUTORCC ON)

set(SOURCES
  src/ros_launcher.cpp
  gui/qml.qrc
  )

add_executable(test_processes src/test_processes.cpp)
target_link_libraries(test_processes ${Boost_LIBRARIES})

add_executable(ros_launcher ${SOURCES})
target_link_libraries(ros_launcher ${Boost_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick)
target_include_directories(ros_launcher PRIVATE ${Qt5Qml_INCLUDE_DIRS})

message("Qt5 INCLUDES:  ${Qt5Qml_INCLUDE_DIRS}")
