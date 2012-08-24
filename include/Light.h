#ifndef __UBERNGINE_LIGHT_H__
#define __UBERNGINE_LIGHT_H__
#include <vector>
#include <Node.h>
#include <glm/glm.hpp>
/*
  This class represents a light that can be added to a scene
*/

namespace Uberngine {

class Light  {
public:
  enum LightType { DIRECTIONAL, POINT, SPOT };
private:
  LightType LT;
  glm::vec4 Direction;
  glm::vec4 Position;
  glm::vec3 Color;

public:
  Light(LightType lt, float x, float y, float z, 
        float dirx, float diry, float dirz,
        float r, float g, float b) : LT(lt), Direction(dirx, diry, dirz, 0.0f), Position(x,y,z, 1.0f),
                                     Color(r,g,b) {} 
  void SetPosition(float x, float y, float z) { Position = glm::vec4(x,y,z, 1.0f); }
  void SetDirection(float dirx, float diry, float dirz) { Direction = glm::vec4(dirx,diry,dirz, 0.0f); }
  void SetColor(float r, float g, float b) { Color = glm::vec3(r,g,b); }
  float *GetDirection() { return glm::value_ptr(Direction); }
  float *GetPosition() { return glm::value_ptr(Position); }
  float *GetColor() { return glm::value_ptr(Color); }
  LightType GetLightType() { return LT; }
};

}

#endif
