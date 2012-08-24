#include <GLFWSystemFacilities.h>
#include <GL/glfw.h>

using namespace Uberngine;

GLFWSystemFacilities::GLFWSystemFacilities() : OGLSystemFacilities(0) { 
  glfwInit(); 
}

bool GLFWSystemFacilities::CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                                     bool fullscreen) {
  int mode = fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW;
  int r_bits, g_bits, b_bits, a_bits;
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

  int ret_val = glfwOpenWindow(*width, *height, r_bits, g_bits, b_bits, a_bits, d_bits, s_bits, mode);
  return (ret_val == GL_TRUE) ? true : false;
}

void GLFWSystemFacilities::GetMousePosition(int *x, int *y) {
  glfwGetMousePos(x, y);
}

bool GLFWSystemFacilities::GetPressedKey(int key) {
  return (glfwGetKey(key) == GLFW_PRESS) ? true : false;
}

float GLFWSystemFacilities::GetTime() { return static_cast<float>(glfwGetTime()); }

void GLFWSystemFacilities::SwapBuffers() { glfwSwapBuffers(); }

void GLFWSystemFacilities::SetWindowTitle(const char *title) { glfwSetWindowTitle(title); }

GLFWSystemFacilities::~GLFWSystemFacilities() { glfwTerminate(); }
