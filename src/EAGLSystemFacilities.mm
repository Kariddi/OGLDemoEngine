#include <EAGLSystemFacilities.h>

using namespace Uberngine;

EAGLSystemFacilities::EAGLSystemFacilities(CAEAGLLayer* layer, CADisplayLink* display_link) : OGLSystemFacilities<EAGLSystemFacilities>(0),
  EAGLLayer(layer), DisplayLink(display_link) {
  
  GLCtx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  if (GLCtx != nil)
    [EAGLContext setCurrentContext:GLCtx];
}

bool EAGLSystemFacilities::CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits,
                                                     bool fullscreen) {
  NSString *ColorFormat = NULL;
  GLuint DepthBufferFormat = 0;
  if (c_bits == 32)
    ColorFormat = kEAGLColorFormatRGBA8;
  else if (c_bits == 16)
    ColorFormat = kEAGLColorFormatRGB565;
  if (d_bits == 16 && s_bits == 0)
    DepthBufferFormat = GL_DEPTH_COMPONENT16;
  else if (d_bits == 24 && s_bits == 8)
    DepthBufferFormat = GL_DEPTH24_STENCIL8_OES;
  else if (d_bits == 24 && s_bits == 0)
    DepthBufferFormat = GL_DEPTH_COMPONENT24_OES;
  
  if (DepthBufferFormat == 0 || ColorFormat == NULL)
    return false;

  if (DrawFramebuffer != 0) {
    glDeleteFramebuffers(1, &DrawFramebuffer);
    glDeleteRenderbuffers(2, Renderbuffers);
  }

  EAGLLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool: NO], 
                                  kEAGLDrawablePropertyRetainedBacking, ColorFormat, kEAGLDrawablePropertyColorFormat,
                                  nil];

  glGenRenderbuffers(2, Renderbuffers);
  glBindRenderbuffer(GL_RENDERBUFFER, Renderbuffers[0]);
  [GLCtx renderbufferStorage:GL_RENDERBUFFER fromDrawable: EAGLLayer];
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, width);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, height);
  
  glBindRenderbuffer(GL_RENDERBUFFER, Renderbuffers[1]);
  glRenderbufferStorage(GL_RENDERBUFFER, DepthBufferFormat, *width, *height);
  glViewport(0,0,*width,*height); 
  return true;
}

// TODO: Implement
CFTimeInterval EAGLSystemFacilities::GetTime() {
  return [DisplayLink timestamp];
}

void EAGLSystemFacilities::SwapBuffers() {
  glBindRenderbuffer(GL_RENDERBUFFER, Renderbuffers[0]);
  [GLCtx presentRenderbuffer:GL_RENDERBUFFER]; 
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void EAGLSystemFacilities::SetWindowTitle(const char *title) {}

EAGLSystemFacilities::~EAGLSystemFacilities() {
  if (DrawFramebuffer != 0) {
    glDeleteFramebuffers(1, &DrawFramebuffer);
    glDeleteRenderbuffers(2, Renderbuffers);
  }
}


