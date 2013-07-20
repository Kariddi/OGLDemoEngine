#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFWSystemFacilities.h>

using namespace Uberngine;

GLFWSystemFacilities::GLFWSystemFacilities() : OGLSystemFacilities<GLFWSystemFacilities>(0),
  window(nullptr) { 

  glfwInit(); 
}

bool GLFWSystemFacilities::CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                                     bool fullscreen) {
  int r_bits, g_bits, b_bits, a_bits;
  GLFWmonitor *monitor = nullptr;
  if (fullscreen) {
    monitor = glfwGetPrimaryMonitor();
  } 
  switch (c_bits) {
    case 32:
      r_bits = 8;
      g_bits = 8;
      b_bits = 8;
      a_bits = 8;
      break;
    case 24:
      r_bits = 8;
      g_bits = 8;
      b_bits = 8;
      a_bits = 0;
      break;
    case 16:
      r_bits = 5;
      g_bits = 6;
      b_bits = 5;
      a_bits = 0;
      break;
    default:
      return false;
  }
  
  glfwWindowHint(GLFW_RED_BITS, r_bits);
  glfwWindowHint(GLFW_GREEN_BITS, g_bits);
  glfwWindowHint(GLFW_BLUE_BITS, b_bits);
  glfwWindowHint(GLFW_ALPHA_BITS, a_bits);
  glfwWindowHint(GLFW_DEPTH_BITS, d_bits);
  glfwWindowHint(GLFW_STENCIL_BITS, s_bits);

  window = glfwCreateWindow(*width, *height, "", monitor, nullptr);
  glfwMakeContextCurrent(window);
  return (window != nullptr) ? true : false;
}

void GLFWSystemFacilities::GetMousePosition(int *x, int *y) {
  double d_x = 0.0, d_y = 0.0;
  glfwGetCursorPos(window, &d_x, &d_y);
  *x = static_cast<int>(d_x);
  *y = static_cast<int>(d_y);
}

void GLFWSystemFacilities::SetPressedKeyCallback(KeyCallbackTy kc) {
  //typedef void(*KeyCallbackPtr)(GLFWwindow*, int, int, int, int);
  glfwSetKeyCallback(window, kc);
}

bool GLFWSystemFacilities::GetPressedKey(int key) {
  return (glfwGetKey(window, key) == GLFW_PRESS) ? true : false;
}

float GLFWSystemFacilities::GetTime() { return static_cast<float>(glfwGetTime()); }

void GLFWSystemFacilities::SwapBuffers() { 
  glfwPollEvents();
  glfwSwapBuffers(window); 
}

void GLFWSystemFacilities::SetWindowTitle(const char *title) { glfwSetWindowTitle(window, title); }

GLFWSystemFacilities::~GLFWSystemFacilities() { glfwTerminate(); }
