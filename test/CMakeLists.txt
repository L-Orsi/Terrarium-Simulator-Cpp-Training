
set(PROJECT_TEST_SRCS
  runner.cpp
  test_bug.cpp
  test_bug_notifier.cpp
  test_map_cell.cpp
  test_simulation_map.cpp
)

enable_testing()

add_executable(${PROJECT_NAME}_test
  ${PROJECT_TEST_SRCS}
    ../src/bug.cpp
    ../src/bug_notifier.cpp
    ../src/map_cell.cpp
    ../src/simulation_engine.cpp
    ../src/simulation_map.cpp
    ../src/bug_descriptor.cpp
)

target_link_libraries(${PROJECT_NAME}_test
  gtest
  gmock
  gtest_main
)

add_test(${PROJECT_NAME}_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_test)
