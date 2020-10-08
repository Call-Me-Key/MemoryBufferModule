cmake_minimum_required (VERSION 3.8)

project(MemoryBufferProject C CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable (MemoryBufferProject "src/main.cpp")

set(SOURCE_LIST
	src/MemoryBuffer.h
	src/MemoryBuffer.cpp
	)
	
target_sources(MemoryBufferProject PRIVATE ${SOURCE_LIST})
