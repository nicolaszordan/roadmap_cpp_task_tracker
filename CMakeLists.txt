cmake_minimum_required(VERSION 3.31.0)

project(task_tracker VERSION 0.1.0 LANGUAGES C CXX)

find_package(Catch2 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall")

set(SOURCES
#   src/task.cpp
#   src/task_tracker.cpp
#   ...
)

add_executable(task_tracker ${SOURCES} src/main.cpp)

target_link_libraries(task_tracker PRIVATE Catch2::Catch2)

# add_executable(task_tracker_tests ${SOURCES})
#
# target_link_libraries(task_tracker_tests PRIVATE Catch2::Catch2WithMain)
#
# include(CTest)
# include(Catch)
# catch_discover_tests(task_tracker_tests)