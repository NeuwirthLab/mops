cmake_minimum_required(VERSION 3.5)

#find_package(Threads REQUIRED)
#find_package(OpenMP REQUIRED)

add_executable(computation "computation.cpp")
target_include_directories(computation PUBLIC "include")
target_compile_features(computation PRIVATE cxx_std_17)
#target_link_libraries(computation PRIVATE "OpenMP::OpenMP_CXX" "Threads::Threads")
