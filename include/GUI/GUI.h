#ifndef __UBERNGINE_GUI_H__
#define __UBERNGINE_GUI_H__
#include <vector>
#include <Texture.h>
#include <Shader.h>
#include <Defs.h>

namespace Uberngine {
class GUIElement;

struct FontInfo {
  GLuint TexObj;
  Shader *FontShader;
  static const char XSize[128];
  static const char XAdvance[128];
  static const float XScale;
  static const float YScale;
  static const int XMaxSize;
  static const int YSize;
  FontInfo() : TexObj(0), FontShader(NULL) {}
  ~FontInfo() { 
    if (TexObj != 0) 
      glDeleteTextures(1, &TexObj); 
    if (FontShader != NULL)
      delete FontShader;
  }
};


class GUI {

typedef std::vector<GUIElement*> ElementVectorTy;
typedef ElementVectorTy::iterator ElementVectorIt;

  ElementVectorTy Elements;
  FontInfo *FInfo;
  GUI();
  void RenderGUI();
  template<PlatformType ty>
  friend class Engine;
public:
  ~GUI();
  const FontInfo *GetFontInfo() const { return FInfo; }
  void AddElement(GUIElement *elem);
};

}

#endif
