
add_library(common STATIC httpUtil.cpp)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(common PRIVATE qlibc)
target_link_libraries(common PUBLIC http)
target_link_libraries(common PRIVATE log)

