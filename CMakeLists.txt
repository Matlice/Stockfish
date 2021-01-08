cmake_minimum_required(VERSION 3.0)
project(stockfish)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_VERBOSE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

ADD_DEFINITIONS(-DUSE_PTHREADS -DNDEBUG -DIS_64BIT -DUSE_POPCNT -DUSE_SSE41 -DUSE_SSSE3 -DUSE_SSE2)

add_compile_options(-Wall -Wcast-qual -fno-exceptions -std=c++17 -pedantic -Wextra -Wshadow -m64 -O0 -msse -msse3 -mpopcnt -msse4.1 -mssse3 -msse2 -flto)

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

add_library(libstockfish STATIC ${src_files})
target_link_libraries(libstockfish pthread)
target_link_options(libstockfish PUBLIC -fPIC)
target_compile_definitions(libstockfish PUBLIC EvalFileDefaultName="${EvalFileDefaultName}")
target_compile_definitions(libstockfish PUBLIC NOMAIN)

add_executable(stockfish_test ${src_files})
target_link_libraries(stockfish_test pthread)
target_compile_definitions(stockfish_test PUBLIC EvalFileDefaultName="${EvalFileDefaultName}")

add_library(libstockfishjni SHARED ${src_files_jni})
target_link_libraries(libstockfishjni pthread libstockfish)
target_link_options(libstockfishjni PUBLIC -fPIC)
target_compile_definitions(libstockfishjni PUBLIC EvalFileDefaultName="${EvalFileDefaultName}")
