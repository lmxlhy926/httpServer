
add_library(http STATIC httplib.cc)
target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(http PRIVATE pthread)
