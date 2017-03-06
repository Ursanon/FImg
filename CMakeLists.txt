cmake_minimum_required(VERSION 3.0)

project(FImg)

add_subdirectory(src)

foreach(testsrc ${ALL_SRC})

    string(REGEX REPLACE "[a-zA-Z:+]*/" "" NAME ${testsrc})
    string(REPLACE ".cpp" "" NAME ${NAME})
    message(${NAME})
    add_executable(${NAME} ${testsrc})
endforeach(testsrc ${ALL_SRC})