project(CLI11)

add_library(cli11 INTERFACE)

target_include_directories(cli11 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)