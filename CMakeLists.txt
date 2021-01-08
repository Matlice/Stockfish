cmake_minimum_required(VERSION 3.0)
project(stockfish)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

ADD_DEFINITIONS(-DUSE_PTHREADS -DIS_64BIT -DNDEBUG -DUSE_POPCNT -DUSE_SSE41 -DUSE_SSSE3 -DUSE_SSE2)

add_compile_options(-Wall -Wcast-qual -fno-exceptions -std=c++17 -pedantic -Wextra -Wshadow -m64 -O3 -msse -msse3 -mpopcnt -msse4.1 -mssse3 -msse2 -flto)

set(EvalFileDefaultName "nn-62ef826d1a6d.nnue")
if (NOT EXISTS ${EvalFileDefaultName})
    message("Downloading ${EvalFileDefaultName}")
    execute_process(COMMAND wget -q "https://tests.stockfishchess.org/api/nn/${EvalFileDefaultName}")
else ()
    message("already there")
endif ()

include_directories(include)
file(GLOB src_files
        "src/nnue/features/*.cpp"
        "src/nnue/*.cpp"
        "src/syzygy/*.cpp"
        "src/interface/*.cpp"
        "src/*.cpp")

file(GLOB src_files_jni
        "src/jni_adapter/*.cpp")

add_library(stockfish STATIC ${src_files})
target_link_libraries(stockfish pthread)
target_compile_definitions(stockfish PUBLIC EvalFileDefaultName="${EvalFileDefaultName}")
target_compile_definitions(stockfish PUBLIC NOMAIN)

add_executable(stockfish_test src/test_exec/main.cpp)
target_link_libraries(stockfish_test pthread stockfish)

add_library(stockfishjni SHARED ${src_files_jni})
target_link_libraries(stockfishjni pthread stockfish)
target_include_directories(stockfishjni PUBLIC stockfish_jni/jni_include)
target_include_directories(stockfishjni PUBLIC /opt/jdk-14.0.1/include/)
target_include_directories(stockfishjni PUBLIC /opt/jdk-14.0.1/include/linux/)

