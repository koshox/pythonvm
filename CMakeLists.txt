cmake_minimum_required(VERSION 3.14)
project(pythonvm)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

INCLUDE_DIRECTORIES(./)

ADD_EXECUTABLE(pythonvm main.cpp)

set(SOURCE_FILE
        object/hiInteger.cpp
        #object/hiDouble.cpp
        object/hiString.cpp
        object/hiObject.cpp
        object/hiList.cpp
        object/hiDict.cpp
        object/klass.cpp
        util/arrayList.cpp
        util/map.cpp
        #util/handles.cpp
        runtime/interpreter.cpp
        runtime/universe.cpp
        runtime/frameObject.cpp
        runtime/functionObject.cpp
        runtime/stringTable.cpp
        runtime/module.cpp
        runtime/traceback.cpp
        #runtime/generator.cpp
        runtime/cellObject.cpp
        memory/heap.cpp
        memory/oopClosure.cpp
        code/binaryFileParser.cpp
        code/codeObject.cpp)

ADD_LIBRARY(pyvm SHARED ${SOURCE_FILE})

ADD_LIBRARY(math SHARED extlib/math.cpp)

ADD_CUSTOM_COMMAND(TARGET math
        POST_BUILD
        COMMAND mkdir -p lib
        COMMAND cp libmath.dll lib/
        COMMAND cp ../lib/*.py lib/
        COMMAND python2 -m compileall lib/*.py
        )

TARGET_LINK_LIBRARIES(pyvm -ldl)
TARGET_LINK_LIBRARIES(math pyvm)

TARGET_LINK_LIBRARIES(pythonvm pyvm ${CMAKE_DL_LIBS})

set_target_properties(pyvm PROPERTIES PREFIX "lib")
set_target_properties(math PROPERTIES PREFIX "lib")