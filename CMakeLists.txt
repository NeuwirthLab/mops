cmake_minimum_required(VERSION 3.20)
project(mops)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

#find_package(Threads REQUIRED)
#find_package(OpenMP REQUIRED)
find_package(TACO REQUIRED)


add_executable(mops "mops.cpp")
target_include_directories(mops PUBLIC "include")
target_compile_features(mops PRIVATE cxx_std_17)
#target_link_libraries(computation PRIVATE "OpenMP::OpenMP_CXX" "Threads::Threads")
target_link_libraries(mops PRIVATE "TACO::TACO")