cmake_minimum_required(VERSION 3.30)
project(fiestatdci09)

set(CMAKE_CXX_STANDARD 23)

add_executable(fiestatdci09 main.cpp SerialInterface.cpp SerialInterface.h Fiesta.cpp Fiesta.h)
add_executable(fiestatdci09-randanalysis randanalysis.cpp seedkey.h)
