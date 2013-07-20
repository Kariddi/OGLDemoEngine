#include <GUI/GUI.h>
#include <GUI/GUIElement.h>
#include <Texture.h>
#include <Shader.h>

using namespace Uberngine;


const char FontInfo::XSize[] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
                                 5, 5, 5, 5, 5, 0, 2, 4, 7, 6, 8, 7, 2, 3, 3, 5, 6, 2, 3, 2, 3, 5, 5, 6, 5, 6, 5, 
                                 5, 5, 5, 5, 2, 2, 7, 7, 7, 5, 9, 6, 6, 6, 6, 5, 5, 6, 6, 2, 2, 6, 5, 7, 6, 6, 5, 
                                 6, 6, 7, 5, 6, 6, 8, 6, 5, 5, 3, 3, 3, 7, 5, 3, 5, 5, 5, 5, 5, 4, 5, 5, 2, 2, 5, 
                                 2, 8, 5, 5, 5, 5, 4, 5, 4, 5, 6, 7, 5, 6, 5, 4, 2, 4, 7, 5 };

const char FontInfo::XAdvance[] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
                                    5, 5, 5, 5, 5, 5, 3, 3, 4, 8, 6, 9, 8, 2, 4, 4, 5, 8, 3, 3, 3, 3, 6, 6, 6, 6, 
                                    6, 6, 6, 6, 6, 6, 3, 3, 8, 8, 8, 5, 10, 6, 7, 7, 7, 6, 6, 7, 7, 3, 3, 6, 5, 8,                                     7, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 5, 5, 4, 3, 4, 8, 5, 5, 6, 6, 6, 6, 6, 3, 6, 
                                    6, 3, 3, 5, 3, 9, 6, 6, 6, 6, 4, 5, 4, 6, 5, 7, 5, 5, 6, 5, 3, 5, 8, 5 };

const float FontInfo::XScale = 0.625f;
const float FontInfo::YScale = 0.625f;
const int FontInfo::XMaxSize = 10;
const int FontInfo::YSize = 10;
 
GUI::GUI() {

  FInfo = new FontInfo;
  Texture *FontTexture = new Texture;
  if (!LoadPNG(FontTexture, "fonts.png")) {
    delete FontTexture;
    delete FInfo;
    FInfo = NULL;
    return;
  }
  glGenTextures(1, &FInfo->TexObj);
  glBindTexture(GL_TEXTURE_2D, FInfo->TexObj);
  glTexImage2D(GL_TEXTURE_2D, 0, FontTexture->format, FontTexture->width, FontTexture->height, 0,
               FontTexture->format, FontTexture->type, FontTexture->imageData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  FInfo->FontShader = new Shader<RendererTypes::OpenGL>("./font.vert", "./font.frag");
  glBindTexture(GL_TEXTURE_2D, 0);

  delete FontTexture;
}

void GUI::RenderGUI() {
  for (ElementVectorIt I = Elements.begin(), E = Elements.end(); I != E; ++I)
    (*I)->Render();
}

GUI::~GUI() {

  for (ElementVectorIt I = Elements.begin(), E = Elements.end(); I != E; ++I)
    delete *I;

  if (FInfo != NULL)
    delete FInfo;
}

void GUI::AddElement(GUIElement *elem) {
  Elements.push_back(elem);
}
