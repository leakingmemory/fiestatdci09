cmake_minimum_required(VERSION 3.30)
project(fiestatdci09)

set(CMAKE_CXX_STANDARD 23)

option(ENABLE_O3 "Build with O3" OFF)
option(ENABLE_NATIVE "Compile for this CPU" OFF)
option(ENABLE_SSE2 "Compile with SSE2" OFF)
option(ENABLE_AVX "Compile with AVX" OFF)
option(ENABLE_AVX512 "Compile with AVX512 - WIP" OFF)
option(NUM_THREADS "Computations run on n threads" 4)

if (ENABLE_O3)
     add_compile_options(-O3)
endif()
if (ENABLE_NATIVE)
    add_compile_options(-march=native)
endif()
if (ENABLE_SSE2)
    add_compile_options(-DUSE_SSE2)
endif()
if (ENABLE_AVX)
    add_compile_options(-DUSE_AVX)
endif()
if (ENABLE_AVX512)
    add_compile_options(-DUSE_AVX512)
endif()
add_compile_options(-DNUM_THREADS=${NUM_THREADS})

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED          glib-2.0>=2.50)
pkg_check_modules(GIO  REQUIRED IMPORTED_TARGET gio-2.0)

add_executable(fiestatdci09 main.cpp SerialInterface.cpp SerialInterface.h Fiesta.cpp Fiesta.h ElmDevice.cpp ElmDevice.h BleDevice.cpp BleDevice.h)
add_executable(fiestatdci09-randanalysis randanalysis.cpp seedkey.h)
add_executable(fiestatdci09-bletest bletest.cpp BleDevice.cpp)

target_link_libraries(fiestatdci09 PRIVATE PkgConfig::GIO)
target_link_libraries(fiestatdci09-bletest PRIVATE PkgConfig::GIO)

target_include_directories(fiestatdci09  PRIVATE
        ${GLIB_INCLUDE_DIRS}
        ${GLIB_LIBRARY_DIRS}
)
target_include_directories(fiestatdci09-bletest  PRIVATE
        ${GLIB_INCLUDE_DIRS}
        ${GLIB_LIBRARY_DIRS}
)

install(TARGETS fiestatdci09 fiestatdci09-randanalysis DESTINATION bin)
