cmake_minimum_required(VERSION 3.13)

file(COPY assets.xml DESTINATION ${CMAKE_BINARY_DIR}/res/)
file(COPY scenes.json DESTINATION ${CMAKE_BINARY_DIR}/res/)

install(FILES assets.xml DESTINATION ./res)
install(FILES scenes.json DESTINATION ./res)
