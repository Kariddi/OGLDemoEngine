#ifndef __UBERNGINE_LIGHT_H__
#define __UBERNGINE_LIGHT_H__
#include <vector>
#include <GlobalTypes.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
/*
  This class represents a light that can be added to a scene
*/

namespace Uberngine {

class Light  {
public:
  enum class LightType { DIRECTIONAL, POINT, SPOT };
private:
  LightType LT;
  glm::vec4 Direction;
  glm::vec4 Position;
  glm::vec3 Color;

public:
  Light(LightType lt, EngineReal x, EngineReal y, EngineReal z, 
        EngineReal dirx, EngineReal diry, EngineReal dirz,
        EngineReal r, EngineReal g, EngineReal b) : LT(lt), Direction(dirx, diry, dirz, 0.0f), Position(x,y,z, 1.0f),
                                     Color(r,g,b) {} 
  void SetPosition(EngineReal x, EngineReal y, EngineReal z) { Position = glm::vec4(x,y,z, 1.0f); }
  void SetDirection(EngineReal dirx, EngineReal diry, EngineReal dirz) { Direction = glm::vec4(dirx,diry,dirz, 0.0f); }
  void SetColor(EngineReal r, EngineReal g, EngineReal b) { Color = glm::vec3(r,g,b); }
  EngineReal *GetDirection() { return glm::value_ptr(Direction); }
  EngineReal *GetPosition() { return glm::value_ptr(Position); }
  EngineReal *GetColor() { return glm::value_ptr(Color); }
  LightType GetLightType() { return LT; }
};

typedef std::vector<Light*> LightList;
typedef LightList::iterator LightListIt;

}

#endif
