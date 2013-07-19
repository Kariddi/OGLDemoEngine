#ifndef __UBERNGINE_RENDERER_H__
#define __UBERNGINE_RENDERER_H__

#include <GlobalTypes.h>
#include <Light.h>
#include <Mesh.h>
#include <Shader.h>

namespace Uberngine {

template<typename Derived>
class Renderer {

public:
  void Render(const EngineReal* view_mat, const EngineReal* proj_mat, 
  	const glm::mat4& comulative, const LightList& lights) const {

  	static_cast<Derived*>(this)->Render(view_mat, proj_mat, comulative, lights);
  }
  void SetMesh(Mesh* mesh) {
  	static_cast<Derived*>(this)->SetMesh(mesh);
  }
  // TODO: This should be changed to Effect, a less render specific class
  void SetShader(Shader* shader) {
    static_cast<Derived*>(this)->SetShader(shader);
  }
//  void SetEffect(Effect* effect) {
//    static_cast<Derived*>(this)->SetEffect(effect);
//  }

};

}

#endif
