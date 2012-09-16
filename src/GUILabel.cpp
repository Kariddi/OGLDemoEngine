#include <GUI/GUILabel.h>
#include <Defs.h>
#include <OGL.h>
#include <cstring>
#include <iostream>

#define VERTEX_COMPONENTS 4

using namespace Uberngine;

GUILabel::GUILabel(GUI *gui) : GUIObj(gui), Text(NULL), VBO(0), VAO(0), VertexNum(0) {}

void GUILabel::SetText(const char *text) {
  if (Text != NULL)
    delete Text;
  int strsize = strlen(text);
  Text = new char[strsize+1];
  Text[strsize] = '\0';
  strncpy(Text, text, strsize);
  const FontInfo *FInfo = GUIObj->GetFontInfo(); 
  float XTextureUnit = FInfo->XScale / 16;
  float YTextureUnit = FInfo->YScale / 8;
  int newline_count = 0;
  for (int i = 0; i < strsize; ++i) {
    if (Text[i] == '\n')
      ++newline_count;
  }

  if (VAO != 0)
    glDeleteVertexArrays(1, &VAO);

  if (VBO != 0)
    glDeleteBuffers(1, &VBO);
 
  uint16_t vertex_size = 6*VERTEX_COMPONENTS;
  VertexNum = (strsize-newline_count)*6;
  uint32_t total_size = vertex_size*(strsize-newline_count);
  float *string_quads_vbo = new float[total_size];
  uint16_t XPen = XPos;
  uint16_t YPen = XPos;
  uint32_t j = 0;
  for (int i = 0; i < strsize; ++i) {
    uint8_t letter = Text[i];
    uint8_t column = letter % 16;
    uint8_t row = letter / 16;
    uint8_t XSize = FInfo->XSize[letter];
    float XTextureStep = ((float) XSize / FInfo->XMaxSize) * XTextureUnit;
    float XTextureStart = XTextureUnit*column;
    float YTextureStart = YTextureUnit*row;
    if (letter == '\n') {
      XPen = XPos;
      YPen += FInfo->YSize;
      continue;
    }
    string_quads_vbo[j] = XPen;
    string_quads_vbo[j+1] = YPen;
    string_quads_vbo[j+2] = XTextureStart;
    string_quads_vbo[j+3] = YTextureStart;
    string_quads_vbo[j+4] = XPen+XSize;
    string_quads_vbo[j+5] = YPen+FInfo->YSize;
    string_quads_vbo[j+6] = XTextureStart+XTextureStep;
    string_quads_vbo[j+7] = YTextureStart+YTextureUnit;
    string_quads_vbo[j+8] = XPen;
    string_quads_vbo[j+9] = YPen+FInfo->YSize;
    string_quads_vbo[j+10] = XTextureStart;
    string_quads_vbo[j+11] = YTextureStart+YTextureUnit;
    string_quads_vbo[j+12] = XPen;
    string_quads_vbo[j+13] = YPen;
    string_quads_vbo[j+14] = XTextureStart;
    string_quads_vbo[j+15] = YTextureStart;
    string_quads_vbo[j+16] = XPen+XSize;
    string_quads_vbo[j+17] = YPen;
    string_quads_vbo[j+18] = XTextureStart+XTextureStep;
    string_quads_vbo[j+19] = YTextureStart;
    string_quads_vbo[j+20] = XPen+XSize;
    string_quads_vbo[j+21] = YPen+FInfo->YSize;
    string_quads_vbo[j+22] = XTextureStart+XTextureStep;
    string_quads_vbo[j+23] = YTextureStart+YTextureUnit;
    XPen += FInfo->XAdvance[letter];
    j += vertex_size;
  }

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, total_size*sizeof(float), string_quads_vbo, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), reinterpret_cast<void*>(0));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), reinterpret_cast<void*>(sizeof(float)*2));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
 
  delete string_quads_vbo;

}

void GUILabel::Render() {
  const FontInfo *FInfo = GUIObj->GetFontInfo();
  GLuint Program = FInfo->FontShader->getProgram();
  glUseProgram(Program);
  GLuint Loc = glGetUniformLocation(Program, "texture");
  glUniform1f(Loc, 0);
  Loc = glGetUniformLocation(Program, "color");
  float color[] = { 1.0f, 1.0f, 1.0f };
  glUniform3fv(Loc, 1, color);
  Loc = glGetUniformLocation(Program, "width");
  glUniform1f(Loc, 640);
  Loc = glGetUniformLocation(Program, "height");
  glUniform1f(Loc, 480);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, FInfo->TexObj);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, VertexNum);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

GUILabel::~GUILabel() {

  if (Text != NULL)
    delete Text;

  if (VAO != 0)
    glDeleteVertexArrays(1, &VAO);

  if (VBO != 0)
    glDeleteBuffers(1, &VBO);
}
