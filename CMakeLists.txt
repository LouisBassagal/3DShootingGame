# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()



# Use file(GLOB ...) to collect all .cpp files in the directory

#execution file name
set(PROJECT_NAME 3DShootingGame)

set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

find_package(FLTK CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

add_executable(
	${PROJECT_NAME}
	source/main.cpp
	source/DrawUtils.cpp
	source/MMatrix.cpp
	source/MyGlWindow.cpp
	source/Object.cpp
	source/stdafx.cpp
	source/timing.cpp
	source/Viewer.cpp
	source/3DUtils.cpp

	source/SimpleScene.cpp

	source/Cyclone/body.cpp
	source/Cyclone/collide_coarse.cpp
	source/Cyclone/collide_fine.cpp
	source/Cyclone/contacts.cpp
	source/Cyclone/core.cpp
	source/Cyclone/fgen.cpp
	source/Cyclone/joints.cpp
	source/Cyclone/particle.cpp
	source/Cyclone/pcontacts.cpp
	source/Cyclone/pfgen.cpp
	source/Cyclone/plinks.cpp
	source/Cyclone/pworld.cpp
	source/Cyclone/random.cpp
	source/Cyclone/world.cpp
 "source/Ball.cpp" "source/Basket.cpp" "source/Box.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE fltk fltk_gl fltk_forms fltk_images glm::glm)
 
