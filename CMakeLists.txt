cmake_minimum_required(VERSION 3.5)

project("Earley Parser")

# Enable all warning by default
set(CMAKE_CXX_FLAGS "-g -Wall")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 3.0.0 REQUIRED)

# MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
# MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")

FIND_PACKAGE( Boost 1.40 COMPONENTS system filesystem REQUIRED )
# INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#
# ADD_EXECUTABLE( anyExecutable myMain.cpp )
#
# TARGET_LINK_LIBRARIES( anyExecutable LINK_PUBLIC ${Boost_LIBRARIES} )

################################
# Normal Libraries & Executables
################################
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(libsvm)
include_directories(hand-recognizer)

# Lib directories
add_subdirectory(hand-recognizer)
add_subdirectory(libsvm)
add_subdirectory(pcfg)

# Temporarily build another executable for the hand recognizer
add_executable(hr hand-main.cpp)
target_link_libraries(hr svm kinectCore ${OpenCV_LIBS})

# This should be the final "global" main
add_executable(gestrec main.cpp)
target_link_libraries(gestrec svm kinectCore ${OpenCV_LIBS})

# Temporarily build another executable for the train
add_executable(train train-main.cpp)
target_link_libraries(train svm kinectCore ${OpenCV_LIBS})

# Temporarily build another executable for the train-testing
add_executable(train-test training-testing.cpp)
target_link_libraries(train-test svm kinectCore ${OpenCV_LIBS})
