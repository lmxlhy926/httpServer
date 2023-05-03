cmake_minimum_required(VERSION 3.23)
project(httpserver)
set(CMAKE_CXX_STANDARD 14)

#设置可执行程序安装路径
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out" CACHE STRING "The path to use for make install" FORCE)
set(TEST_ENABLE true)

add_subdirectory(http)
add_subdirectory(siteService)
add_subdirectory(spdlog)
add_subdirectory(log)

add_executable(httpServer httpServer.cpp)
target_link_libraries(httpServer PRIVATE log)
target_link_libraries(httpServer PRIVATE siteService)

#install
install(TARGETS httpServer  DESTINATION bin)