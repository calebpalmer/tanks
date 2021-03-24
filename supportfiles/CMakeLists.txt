cmake_minimum_required(VERSION 3.13)

configure_file(scenes.json ${CMAKE_BINARY_DIR}/res/scenes.json)
configure_file(assets.xml ${CMAKE_BINARY_DIR}/res/scenes.json)

install(FILES assets.xml DESTINATION ./res)
install(FILES scenes.json DESTINATION ./res)
