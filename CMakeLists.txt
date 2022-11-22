cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)
project(cxx_simple_reflection LANGUAGES C CXX VERSION 0.0.0)

include(FetchContent)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# aether-game-utils dependency
FetchContent_Declare(
	aether-game-utils
	GIT_REPOSITORY "https://github.com/johnhues/aether-game-utils.git"
	GIT_TAG be54d18e15da9076686fb221b85263989425c156
)
set(AE_LEAN_AND_MEAN ON CACHE INTERNAL "") ## Enabled to skip unit tests and examples
FetchContent_MakeAvailable(aether-game-utils)

# imgui dependency
FetchContent_Declare(
	imgui
	GIT_REPOSITORY "https://github.com/johnhues/imgui.git"
	GIT_TAG b85f0ee12fc8aa62b4bc571dd3249d82c7ac2550
)
FetchContent_MakeAvailable(imgui)

add_executable(cxx_simple_reflection main.cpp cxx_simple_reflection.h)
target_link_libraries(cxx_simple_reflection ae imgui)
