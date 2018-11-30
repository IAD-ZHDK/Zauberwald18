# minimal required cmake version
cmake_minimum_required(VERSION 3.7)

# you can set your own project name
project(Zauberwald18)

# this should not be changed
set(CMAKE_C_STANDARD 99)

# add your source files
set(SOURCE_FILES
        src/anemometer.h
        src/anemoteter.c
        src/APA102.h
        src/APA102.c
        src/enc.c
        src/enc.h
        src/main.c
        src/mot.c
        src/mot.h
        src/neo3.c
        src/neo3.h
        src/neo4.c
        src/neo4.h
        src/neoPixelStandard.c
        src/neoPixelStandard.h
        src/object1.h
        src/object1.c
        src/object2.h
        src/object2.c
        src/object3.h
        src/object3.c
        src/object4.h
        src/object4.c
        src/servo.h
        src/servo.c)

# create a fake library target
add_library(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# include naos include paths
add_subdirectory(naos)
