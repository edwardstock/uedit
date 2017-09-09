cmake_minimum_required(VERSION 3.0)
project(uedit)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER_FLAGS "-Werror -Wfatal-error -pedantic")
set(CMAKE_CXX_LINKER_FLAGS "-static-libstdc++")

#if (NOT APPLE)
#	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
#	set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
#	set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
#elseif (APPLE)
#	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -Xarch_x86_64 -mmacosx-version-min=10.7")
#endif()

set(PROJECT_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/libs
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
    src/editor/Search.cpp
    src/editor/Search.h definitions.h)

add_executable(uedit ${SOURCE_FILES})

target_link_libraries(uedit cpphelpers)