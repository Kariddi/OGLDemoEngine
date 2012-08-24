#ifndef __UBERNGINE_SYSTEMFACILITIESFACTORY_H__
#define __UBERNGINE_SYSTEMFACILITIESFACTORY_H__

#include <SystemFacilities.h>
#include <Defs.h>
#if defined(MAC_SYS) || defined(_WIN32) || defined(_linux) 
#include <GLFWSystemFacilities.h>
#endif
#if defined(IOS_SYS)
#include <EAGLSystemFacilities.h>
#endif

//class UIView;

namespace Uberngine {

class BaseSystemFacilitiesFactory {
protected:
  static SystemFacilities *Sys;
  
public:
  static void Destroy() { if (Sys) delete Sys; }

};

template<PlatformType platform>
class SystemFacilitiesFactory : public BaseSystemFacilitiesFactory { SystemFacilitiesFactory<platform>() {} };

}

#endif
