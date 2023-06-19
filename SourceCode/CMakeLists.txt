cmake_minimum_required(VERSION 3.10)
project(LabEscape)

set(CMAKE_C_STANDARD 11)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Set new executable
add_executable(${CMAKE_PROJECT_NAME}
	game.c
	M3DReader.c
	M3Dtranslator.c
	main.c
	opengl_engine.c
)

# Target
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
        # ...
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
	${OPENGL_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
)

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
)

target_link_libraries(${CMAKE_PROJECT_NAME}
	${OPENGL_LIBRARIES}
	m
	glut
	GLU
)

# Custom output directory
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME})
add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}
		./LabEscape -translate ${CMAKE_SOURCE_DIR}/../BlenderModels/gamemap3.obj map.m3d
	DEPENDS ${CMAKE_PROJECT_NAME}
	COMMENT "Generating map.m3d"
)
