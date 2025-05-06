project(wayland_fix)

add_library(wayland_fix SHARED main.cpp)

target_compile_features(wayland_fix PUBLIC cxx_std_23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include("FetchContent")

FetchContent_Declare(
	MemoryManager
	GIT_REPOSITORY https://github.com/Sumandora/MemoryManager
	GIT_PROGRESS TRUE
	GIT_TAG 2.5.1)
FetchContent_MakeAvailable(MemoryManager)

add_subdirectory("${MemoryManager_SOURCE_DIR}/Modules/Linux")
target_link_libraries(wayland_fix PRIVATE LinuxMemoryManager)

