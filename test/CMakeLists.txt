add_executable(${PROJECT_NAME}_tests ssm_tests.cpp another_unit.cpp)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME})
