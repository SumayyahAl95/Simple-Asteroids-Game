cmake_minimum_required(VERSION 3.14)
project(src)

set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(src main.cpp Game.hpp Game.cpp Asteroid.cpp Asteroid.hpp Bullet.cpp Bullet.hpp Player.hpp)
target_link_libraries(src sfml-graphics sfml-audio)