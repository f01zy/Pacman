cmake_minimum_required(VERSION 3.5)
project(Pacman)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(SOURCE 
  ./src/main.c
  ./src/level.c
  ./src/pacman.c
  ./src/position.c
  ./src/input.c
  ./src/hud.c
  ./src/text.c
)

add_subdirectory(./third_party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(./third_party/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(./third_party/SDL_ttf EXCLUDE_FROM_ALL)

add_executable(pacman ${SOURCE})
target_link_libraries(pacman PRIVATE
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  SDL3::SDL3
  m
)
