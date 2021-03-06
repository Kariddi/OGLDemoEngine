cmake_minimum_required (VERSION 2.6)
project (Demo)

include_directories ("${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE INCS "include/*.h")

if (NOT IOS)

FIND_PACKAGE(Bullet REQUIRED)
SET(EXTRA_LIBS ${EXTRA_LIBS} ${BULLET_LIBRARIES})
INCLUDE_DIRECTORIES(${BULLET_INCLUDE_DIR})
FIND_PACKAGE(PNG REQUIRED)
SET(EXTRA_LIBS ${EXTRA_LIBS} ${PNG_LIBRARIES})
INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})

else (NOT IOS)

#SET(EXTRA_LIBS ${EXTRA_LIBS} ${BULLET_MATH_LIBRARY} ${BULLET_SOFTBODY_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} )
#INCLUDE_DIRECTORIES(${BULLET_INCLUDE_DIR})

#SET(EXTRA_LIBS ${EXTRA_LIBS} ${PNG_LIBRARY})
#INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})

endif (NOT IOS)

SET(COMMON_FILES src/TextureData.cpp src/Shader.cpp src/PhysicsManager.cpp src/RigidBody.cpp src/CollisionShape.cpp src/Scene.cpp src/Node.cpp src/GUI.cpp src/GUILabel.cpp src/Camera.cpp src/ObjMaterialLoader.cpp)
SET(OPENGL_FILES src/GLRenderingSurface.cpp src/GLTextureTarget.cpp src/GLBufferTarget.cpp)
SET(GLFW_FILES src/GLFWSystemFacilities.cpp src/GLFWEngine.cpp)
SET(IOS_FILES src/EAGLSystemFacilities.mm src/IOSEngine.cpp)
SET(compile_flags "${compile_flags} -std=c++11")

if (APPLE AND (NOT IOS))
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL )
  FIND_LIBRARY(IOKIT_LIBRARY IOKit )
  MARK_AS_ADVANCED (COCOA_LIBRARY
                    OPENGL_LIBRARY
                    IOKIT_LIBRARY)
  SET(EXTRA_LIBS ${EXTRA_LIBS} ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})
  ADD_LIBRARY(glfw STATIC IMPORTED)
  SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ./libglfwmac.a)
endif (APPLE AND (NOT IOS))

if (UNIX AND (NOT APPLE))
FIND_PACKAGE(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)
INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
SET(CMAKE_CXX_LINK_EXECUTABLE
   "${CMAKE_CXX_LINK_EXECUTABLE} -lrt -lX11 -lXrandr -lGLU -lGL -lGLEW -pthread -lm")
endif (UNIX AND (NOT APPLE))

if (WIN32)
#ADD_LIBRARY(glfw STATIC IMPORTED)
ADD_LIBRARY(opengl32 STATIC IMPORTED)
FIND_LIBRARY(GLFW_LIBRARY glfw "${PROJECT_SOURCE_DIR}/")
#SET_TARGET_PROPERTIES(glfw PROPERTIES
#    IMPORTED_LOCATION GLFW.lib)
SET_TARGET_PROPERTIES(opengl32 PROPERTIES
    IMPORTED_LOCATION opengl32.lib)
endif (WIN32)

if (UNIX AND (NOT APPLE)) 
ADD_EXECUTABLE(Demo ${OPENGL_FILES} ${GLFW_FILES} ${COMMON_FILES}  main.cpp)
TARGET_LINK_LIBRARIES(Demo ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
endif (UNIX AND (NOT APPLE))

if (WIN32) 
ADD_EXECUTABLE(Demo src/glew.c ${OPENGL_FILES} ${GLFW_FILES} ${COMMON_FILES} main_win32.cpp ${INCS})
endif (WIN32)

if (APPLE AND (NOT IOS))
ADD_EXECUTABLE(Demo ${OPENGL_FILES} ${GLFW_FILES} ${COMMON_FILES} main.cpp ${INCS})
if (CMAKE_BUILD_TYPE MATCHES "Profile")
  #SET_TARGET_PROPERTIES(Demo PROPERTIES COMPILE_FLAGS "-pg" ${COMPILE_FLAGS})
  #SET(link_flags "${link_flags} -lprofiler")
  #SET_TARGET_PROPERTIES(Demo PROPERTIES LINK_FLAGS "-lprofiler" ${LINK_FLAGS})
  #SET_TARGET_PROPERTIES(Demo PROPERTIES COMPILE_FLAGS "-g -O3" ${COMPILE_FLAGS})
  SET(compile_flags "${compile_flags} -pg")
  SET(link_flags "${link_flags} -lprofiler")
endif (CMAKE_BUILD_TYPE MATCHES "Profile")
SET(compile_flags "${compile_flags} -stdlib=libc++")
SET(link_flags "${link_flags} -lc++")
endif (APPLE AND (NOT IOS))

if (APPLE AND IOS)
ADD_LIBRARY(Uberngine STATIC ${OPENGL_FILES} ${IOS_FILES} ${COMMON_FILES} ${INCS}) 
SET(compile_flags "${compile_flags} -stdlib=libc++")
SET(link_flags "${link_flags} -lc++")
SET(compile_flags "${compile_flags} -x objective-c++ -fobjc-arc")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
  SET_TARGET_PROPERTIES(Uberngine PROPERTIES XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
endif (CMAKE_BUILD_TYPE MATCHES "Debug")

SET_TARGET_PROPERTIES(Uberngine PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")

SET_TARGET_PROPERTIES(Uberngine PROPERTIES COMPILE_FLAGS ${compile_flags})
SET_TARGET_PROPERTIES(Uberngine PROPERTIES LINK_FLAGS ${link_flags})
endif(APPLE AND IOS)

if (NOT IOS)
if (APPLE)
  SET_TARGET_PROPERTIES(Demo PROPERTIES COMPILE_FLAGS ${compile_flags})
  SET_TARGET_PROPERTIES(Demo PROPERTIES LINK_FLAGS ${link_flags})
endif (APPLE)

SET_TARGET_PROPERTIES(Demo PROPERTIES COMPILE_FLAGS ${compile_flags})
endif (NOT IOS)

if (APPLE AND IOS)
target_link_libraries(Uberngine ${EXTRA_LIBS})
elseif (UNIX)
target_link_libraries(Demo glfw ${EXTRA_LIBS})
endif (APPLE AND IOS)

if (WIN32)
ADD_DEFINITIONS(/DGLEW_STATIC)
target_link_libraries(Demo opengl32 ${GLFW_LIBRARY} ${EXTRA_LIBS})
endif (WIN32)
