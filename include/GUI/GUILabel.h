#ifndef __UBERNGINE_GUILABEL_H__
#define __UBERNGINE_GUILABEL_H__
#include <GUI/GUI.h>
#include <GUI/GUIElement.h>
#include <OGL.h>

namespace Uberngine {

class GUILabel : public GUIElement {

  GUI *GUIObj;
  char *Text;
  GLuint VBO;
  GLuint VAO;
  uint16_t VertexNum;
public:
  GUILabel(GUI *gui);
  void SetText(const char *text);
  virtual void Render();
  virtual ~GUILabel();
};

}

#endif
