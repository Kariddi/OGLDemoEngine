#include <GLFWSystemFacilities.h>
#include <GL/glfw.h>

using namespace Uberngine;

GLFWSystemFacilities::GLFWSystemFacilities() : OGLSystemFacilities(0) { 
  glfwInit(); 
}

bool GLFWSystemFacilities::CreateAndSetRenderContext(int width, int height, int r_bits, int g_bits, 
                                                     int b_bits, int a_bits, int d_bits, int s_bits, bool fullscreen) {
  int mode = fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW;
  int ret_val = glfwOpenWindow(width, height, r_bits, g_bits, b_bits, a_bits, d_bits, s_bits, mode);
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
