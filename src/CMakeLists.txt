aux_source_directory(. SRC_LIST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin )
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_executable(client client.cpp)
target_link_libraries(client gtest pthread)


