#ifndef __UBERNGINE_GUIELEMENT_H__
#define __UBERNGINE_GUIELEMENT_H__

#include <Defs.h>

namespace Uberngine {

class GUIElement {

protected:
  uint16_t XPos;
  uint16_t YPos;

public:
  void SetXPos(uint16_t xpos) { XPos = xpos; }
  void SetYPos(uint16_t ypos) { YPos = ypos; }
  uint16_t GetXPos() { return XPos; }
  uint16_t GetYPos() { return YPos; }
  virtual void Render() = 0;
  GUIElement() : XPos(0), YPos(0) {}
  virtual ~GUIElement() {};
};

}

#endif
