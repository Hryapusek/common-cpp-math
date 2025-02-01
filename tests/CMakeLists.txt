message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} tests")

set(CATCH2_TEST_NAME ${PROJECT_NAME}_tests)

add_executable(${CATCH2_TEST_NAME})

target_link_libraries(${CATCH2_TEST_NAME} PRIVATE Catch2::Catch2 ${PROJECT_NAME}::${PROJECT_NAME})

target_include_directories(${CATCH2_TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_sources(${CATCH2_TEST_NAME} 
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test-rotations.cc
)

message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} tests done_s0!")
