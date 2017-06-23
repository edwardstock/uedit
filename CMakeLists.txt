cmake_minimum_required(VERSION 3.0)
project(uedit)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER_FLAGS "-Werror -Wfatal-error -pedantic")

set(PROJECT_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/libs
    ${PROJECT_SOURCE_DIR}/src/helpers
    ${PROJECT_SOURCE_DIR}/libs/args
    )

include_directories("${PROJECT_INCLUDE_DIR}")

set(SOURCE_FILES
    main.cpp
    includes.h
    src/editor/Editor.cpp
    src/editor/Editor.h
    src/parser/FileParser.h
    src/parser/IniParser.cpp
    src/parser/IniParser.h
    src/parser/JsonParser.cpp
    src/parser/JsonParser.h
    src/parser/ConfParser.cpp
    src/parser/ConfParser.h
    src/parser/LineParser.cpp
    src/parser/LineParser.h
    libs/json.hpp
    ${PROJECT_INCLUDE_DIR}
    src/editor/Search.cpp src/editor/Search.h definitions.h)
add_executable(uedit ${SOURCE_FILES})