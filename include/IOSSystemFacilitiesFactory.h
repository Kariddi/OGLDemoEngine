#ifndef __UBERNGINE_IOSSYSTEMFACILITIESFACTORY_H__
#define __UBERNGINE_IOSSYSTEMFACILITIESFACTORY_H__

#include <SystemFacilitiesFactory.h>

namespace Uberngine {

template<>
class SystemFacilitiesFactory<UBE_IOS> : public BaseSystemFacilitiesFactory {
public:
  static SystemFacilities *Create(UIView *view) {
    if (!Sys)
      Sys = new EAGLSystemFacilities(view);
    return Sys;
  }
};

}
#endif
