#ifndef __UBERNGINE_GLFWSYSTEMFACILITIESFACTORY_H__
#define __UBERNGINE_GLFWSYSTEMFACILITIESFACTORY_H__
#include <SystemFacilitiesFactory.h>

namespace Uberngine {

template<>
class SystemFacilitiesFactory<UBE_GLFW> : public BaseSystemFacilitiesFactory {
public:
  static SystemFacilities *Create() {
    if (!Sys)
      Sys = new GLFWSystemFacilities;
    return Sys;
  }

};

}
#endif
