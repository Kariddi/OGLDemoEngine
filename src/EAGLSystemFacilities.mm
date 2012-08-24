#include <EAGLSystemFacilities.h>

using namespace Uberngine;

EAGLSystemFacilities::EAGLSystemFacilities(UIView *view) : OGLSystemFacilities(0), IosView(view) {
  GLCtx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
}

bool EAGLSystemFacilities::CreateAndSetRenderContext(int width, int height, int r_bits, 
                                              int g_bits, int b_bits, int a_bits, int d_bits, int s_bits, bool mode) {
  return false;
}

void EAGLSystemFacilities::GetMousePosition(int *x, int *y) {}

bool EAGLSystemFacilities::GetPressedKey(int key) { return false; }

float EAGLSystemFacilities::GetTime() { return 0.0f; }

void EAGLSystemFacilities::SwapBuffers() {}

void EAGLSystemFacilities::SetWindowTitle(const char *title) {}

EAGLSystemFacilities::~EAGLSystemFacilities() {}


