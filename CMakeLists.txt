cmake_minimum_required(VERSION 3.12)
project(ParallelMatrixMultiplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(matmul matmul.cpp)

target_link_libraries(matmul PRIVATE OpenMP::OpenMP_CXX)

if(MSVC)
    target_compile_options(matmul PRIVATE /openmp)
endif()