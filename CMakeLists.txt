# CMakeList.txt : CMake project for obj_loader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("obj_loader")

# Add source to this project's executable.
add_executable (
	obj_loader 
	"src/main.cpp" 
	
 "src/engine.cpp" "include/engine.h")

add_subdirectory(third_party)

target_include_directories(
        obj_loader
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
        obj_loader
        PRIVATE
        glad
        glfw
        imgui
)

target_compile_features(obj_loader PUBLIC cxx_std_20)