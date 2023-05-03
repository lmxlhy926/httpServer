
add_library(siteService STATIC cJSON.c service_site_manager.cpp)
target_include_directories(siteService PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(siteService PUBLIC nlohmann)
target_link_libraries(siteService PRIVATE http)
