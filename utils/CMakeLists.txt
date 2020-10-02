add_executable(use_asio use_asio.cpp)
target_link_libraries(use_asio PRIVATE proj-core)

add_executable(use_fmt use_fmt.cpp)
target_link_libraries(use_fmt PRIVATE proj-core)

add_executable(use_json use_json.cpp)
target_link_libraries(use_json PRIVATE proj-core)

add_executable(use_sml2 use_sml2.cpp)
target_link_libraries(use_sml2 PRIVATE proj-core)

add_executable(use_spdlog use_spdlog.cpp)
target_link_libraries(use_spdlog PRIVATE proj-core)

add_executable(use_url use_url.cpp)
target_link_libraries(use_url PRIVATE proj-core)
