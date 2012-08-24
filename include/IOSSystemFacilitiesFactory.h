#ifndef __UBERNGINE_IOSSYSTEMFACILITIESFACTORY_H__
#define __UBERNGINE_IOSSYSTEMFACILITIESFACTORY_H__

#include <SystemFacilitiesFactory.h>

namespace Uberngine {

template<>
class SystemFacilitiesFactory<UBE_IOS> : public BaseSystemFacilitiesFactory {
public:
  static SystemFacilities *Create(CAEAGLLayer *layer) {
    if (!Sys)
      Sys = new EAGLSystemFacilities(layer);
    return Sys;
  }
};

}
#endif
