cmake_minimum_required(VERSION 3.20)
project(mops)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

option(WITH_OPENMP "OpenMP" OFF)
option(WITH_TACO "TACO" OFF)
option(WITH_LIKWID "Likwid" OFF)

if(WITH_LIKWID)
	set(WITH_OPENMP "ON")
	find_package(likwid REQUIRED COMPONENTS marker)
endif()

if(WITH_OPENMP)
	find_package(Threads REQUIRED)
	find_package(OpenMP REQUIRED)
endif()

if(WITH_TACO)
	find_package(TACO REQUIRED)
endif()


add_executable(mops "mops.cpp")
target_include_directories(mops PUBLIC "include" "$<$<BOOL:${WITH_LIKWID}>:${LIKWID_INCLUDE_DIRS}>")
target_compile_features(mops PRIVATE cxx_std_17)
target_link_libraries(mops PRIVATE "$<$<BOOL:${WITH_OPENMP}>:OpenMP::OpenMP_CXX>" "$<$<BOOL:${WITH_OPENMP}>:Threads::Threads>" "$<$<BOOL:${WITH_TACO}>:TACO::TACO>" "$<$<BOOL:${WITH_LIKWID}>:likwid::likwid>")
target_compile_definitions(mops PRIVATE "$<$<BOOL:${WITH_OPENMP}>:WITH_OPENMP>" "$<$<BOOL:${WITH_TACO}>:WITH_TACO>" "$<$<BOOL:${WITH_LIKWID}>:WITH_LIKWID>" "$<$<CONFIG:Debug>:DEBUG>")
