cmake_minimum_required(VERSION 3.5)

project("Earley Parser")

# Enable all warning by default
set(CMAKE_CXX_FLAGS "-g -Wall")

################################
# Normal Libraries & Executables
################################
include_directories(jama)
include_directories(earley)

add_subdirectory(earley)

add_executable(myearley main.cpp)

# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.
target_link_libraries(myearley earleyCore)

################################
# Testing
################################
if (test)

  # Testing through Googletest; addedd as a submodule
  # to this git repository
  # git submodule add https://github.com/google/googletest.git gtest
  include(gtest.cmake)

  # GTest needs threading support
  find_package (Threads)
  target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(gtest_main ${CMAKE_THREAD_LIBS_INIT})

  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)

  enable_testing()

  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests test_earley.cc)

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # Extra linking for the project.
  # target_link_libraries(runUnitTests gi_lib)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  add_test(NAME that-test-I-made COMMAND runUnitTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  # add_test(that-other-test-I-made runUnitTests)
endif()
