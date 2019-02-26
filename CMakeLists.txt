cmake_minimum_required(VERSION 3.5)

project("Earley Parser")

# Enable all warning by default
set(CMAKE_CXX_FLAGS "-g -Wall")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(jama)
include_directories(earley)

# Lib directories
add_subdirectory(earley)


# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.


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

# Build probabilistic parser executable
add_executable(myearley ./earley/earley-main.cpp)

# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.
target_link_libraries(myearley kinectCore earleyCore)

# Lib directories
add_subdirectory(hand-recognizer)
add_subdirectory(libsvm)

# Temporarily build another executable for the train
add_executable(train train-main.cpp)
target_link_libraries(train svm kinectCore ${OpenCV_LIBS})

# Temporarily build another executable
add_executable(main main.cpp)
target_link_libraries(main svm kinectCore earleyCore ${OpenCV_LIBS})
