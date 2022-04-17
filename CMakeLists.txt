cmake_minimum_required(VERSION 3.13)
project(tanks)

include(GNUInstallDirs)

#set( CMAKE_VERBOSE_MAKEFILE on )

add_subdirectory(capengine)
add_subdirectory(supportfiles)

if (MSVC)
  add_compile_options(/W4)
else()
  #add_compile_options(-ggdb3 -Wall -Wextra -Wpedantic -Wno-reorder -Wno-unused -std=c++17)
  add_compile_options(-ggdb3 -Wall -Wextra -Wnon-virtual-dtor -Wshadow -Wpedantic -Wno-reorder -Wno-unused -std=c++17 -fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()

add_definitions(-DUNIX -DDEBUG)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${LIBXML2_INCLUDE_DIR}
  ${CPPUNIT_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/capengine/jsoncons/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/capengine"
  )

include_directories(
  ${Boost_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/capengine/jsoncons/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/capengine"
  )

add_executable(tanks
  game.cpp
  graphicscomponent.cpp
  playerinputcomponent.cpp
  tankphysicscomponent.cpp
  projectilephysicscomponent.cpp
  startmenustate.cpp
  pausestate.cpp
  playercomponent.cpp
  gameevents.cpp
  tankscenestate.cpp
  )

target_link_libraries(tanks capengine)

file(COPY assets/res DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY supportfiles/assets.xml DESTINATION ${CMAKE_BINARY_DIR}/bin/res)
file(COPY supportfiles/scenes.json DESTINATION ${CMAKE_BINARY_DIR}/bin/res)
install(DIRECTORY assets/res/images DESTINATION ./res)

# add target for support files
file(GLOB ConfigFiles ${CMAKE_SOURCE_DIR}/assets/res/* ${CMAKE_SOURCE_DIR}/supportfiles/*)
add_custom_target(copy)
foreach(ConfigFile ${ConfigFiles})
  add_custom_command(TARGET copy PRE_BUILD
		     COMMAND ${CMAKE_COMMAND} -E
		     copy ${ConfigFile} ${CMAKE_BINARY_DIR}/bin/res)
endforeach()
add_dependencies(tanks copy)


install(
  TARGETS tanks capengine
  DESTINATION ./)

install(FILES scripts/tanks.sh
  DESTINATION ./
  PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
