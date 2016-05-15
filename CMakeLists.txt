cmake_minimum_required(VERSION 3.3)
project(CSC305_A1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp SceneObject.h Vect3.h Sphere.h Ray.h Scene.h RGB_Color.h Camera.h Plane.h)
set(OPENGL_LIB C:/MinGW/lib/libopengl32.a)
set(FREEGLUT_LIB C:/MinGW/lib/libfreeglut.a)

add_executable(CSC305_A1 ${SOURCE_FILES})

target_link_libraries(CSC305_A1 ${OPENGL_LIB} ${FREEGLUT_LIB})