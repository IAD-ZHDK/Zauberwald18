# minimal required cmake version
cmake_minimum_required(VERSION 3.7)

# you can set your own project name
project(naos-project)

# this should not be changed
set(CMAKE_C_STANDARD 99)

# add your source files
set(SOURCE_FILES src/main.c src/enc.c src/enc.h src/mot.c src/mot.h src/neo.c src/neo.h src/anemometer.h src/anemoteter.c)

# create a fake library target
add_library(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# include naos include paths
add_subdirectory(naos)
