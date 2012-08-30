cmake_minimum_required (VERSION 2.6)
project (Demo)

include_directories ("${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE INCS "include/*.h")

if (APPLE AND (NOT IOS))
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL )
  FIND_LIBRARY(IOKIT_LIBRARY IOKit )
  MARK_AS_ADVANCED (COCOA_LIBRARY
                    OPENGL_LIBRARY
                    IOKIT_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})
  ADD_LIBRARY(glfw STATIC IMPORTED)
  SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ./libglfwmac.a)
endif (APPLE AND (NOT IOS))

if (UNIX AND (NOT APPLE))
ADD_LIBRARY(glfw STATIC IMPORTED)
SET_TARGET_PROPERTIES(glfw PROPERTIES
    IMPORTED_LOCATION ./libglfw.a)
SET(CMAKE_CXX_LINK_EXECUTABLE
   "${CMAKE_CXX_LINK_EXECUTABLE} -lrt -lX11 -lXrandr -lGLU -lGL -pthread -lm")
endif (UNIX AND (NOT APPLE))

if (WIN32)
ADD_LIBRARY(glfw STATIC IMPORTED)
ADD_LIBRARY(opengl32 STATIC IMPORTED)
SET_TARGET_PROPERTIES(glfw PROPERTIES
    IMPORTED_LOCATION GLFW.lib)
SET_TARGET_PROPERTIES(opengl32 PROPERTIES
    IMPORTED_LOCATION opengl32.lib)
endif (WIN32)

if (UNIX AND (NOT APPLE)) 
ADD_EXECUTABLE(Demo src/glew.c src/GLFWSystemFacilities.cpp src/SystemFacilitiesFactory.cpp src/Texture.cpp src/Engine.cpp src/Scene.cpp src/Shader.cpp src/Node.cpp src/Camera.cpp src/Loader.cpp src/MaterialLoader.cpp main.cpp)
endif (UNIX AND (NOT APPLE))

if (WIN32) 
ADD_EXECUTABLE(Demo src/glew.c src/GLFWSystemFacilities.cpp src/SystemFacilitiesFactory.cpp src/Texture.cpp src/Engine.cpp src/Scene.cpp src/Shader.cpp src/Node.cpp src/Camera.cpp src/Loader.cpp src/MaterialLoader.cpp main_win32.cpp ${INCS})
endif (WIN32)

if (APPLE AND (NOT IOS))
ADD_EXECUTABLE(Demo src/GLFWSystemFacilities.cpp src/SystemFacilitiesFactory.cpp src/Texture.cpp src/Engine.cpp src/Scene.cpp src/Shader.cpp src/Node.cpp src/Camera.cpp src/Loader.cpp src/MaterialLoader.cpp main.cpp ${INCS})
SET_TARGET_PROPERTIES(Demo PROPERTIES COMPILE_FLAGS "-O0 -g")
endif (APPLE AND (NOT IOS))

if (APPLE AND IOS)
ADD_LIBRARY(Uberngine STATIC src/EAGLSystemFacilities.mm src/SystemFacilitiesFactory.cpp src/Texture.cpp src/Engine.cpp src/Scene.cpp src/Shader.cpp src/Node.cpp src/Camera.cpp src/Loader.cpp src/MaterialLoader.cpp ${INCS}) 
SET_TARGET_PROPERTIES(Uberngine PROPERTIES COMPILE_FLAGS "-x objective-c++ -fobjc-arc")
  if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET_TARGET_PROPERTIES(Uberngine PROPERTIES XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
  endif (CMAKE_BUILD_TYPE MATCHES "Debug")

SET_TARGET_PROPERTIES(Uberngine PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
endif(APPLE AND IOS)

if (APPLE AND IOS)
target_link_libraries(Uberngine ${EXTRA_LIBS})
else (APPLE AND IOS)
target_link_libraries(Demo glfw ${EXTRA_LIBS})
endif (APPLE AND IOS)
if (WIN32)
ADD_DEFINITIONS(/DGLEW_STATIC)
target_link_libraries(Demo opengl32 ${EXTRA_LIBS})
endif (WIN32)
