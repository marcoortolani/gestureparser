cmake_minimum_required(VERSION 3.10)

project("Earley Parser")

# Enable all warning by default
set(CMAKE_CXX_FLAGS "-g -Wall")

################################
# Normal Libraries & Executables
################################
# include_directories(${CMAKE_SOURCE_DIR}/jama)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jama)
include_directories(jama)
include_directories(earley)

add_subdirectory(earley)

add_executable(myearley main.cpp)

# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.

target_link_libraries(myearley earleyCore)

# ################################
# # Testing
# ################################
# if (test)
#   if (APPLE)
#     add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
#     add_definitions(-D__GLIBCXX__)
#   endif (APPLE)
#
#   # This adds another subdirectory, which has 'project(gtest)'.
# #  add_subdirectory(lib/gtest-1.6.0)
#
#   # This adds another subdirectory, which has 'project(googletest-distribution)'.
#   add_subdirectory(lib/googletest-master/googletest)
#
#   enable_testing()
#
#   # Include the gtest library. gtest_SOURCE_DIR is available due to
#   # 'project(gtest)' above.
#   include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#
#   ##############
#   # Unit Tests
#   ##############
#   add_executable(runUnitTests test_project1.cpp)
#
#   # Standard linking to gtest stuff.
#   target_link_libraries(runUnitTests gtest gtest_main)
#
#   # Extra linking for the project.
#   target_link_libraries(runUnitTests project1_lib)
#
#   # This is so you can do 'make test' to see all your tests run, instead of
#   # manually running the executable runUnitTests to see those specific tests.
#   add_test(NAME that-test-I-made COMMAND runUnitTests)
#
#   # You can also omit NAME and COMMAND. The second argument could be some other
#   # test executable.
#   add_test(that-other-test-I-made runUnitTests)
# endif()