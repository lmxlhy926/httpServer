FILE(GLOB src "*.cpp")
add_library(log STATIC ${src})
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(log PUBLIC spdlog)