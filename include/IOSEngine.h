#ifndef __UBERNGINE_IOSENGINE_H__
#define __UBERNGINE_IOSENGINE_H__
#include <IOSSystemFacilitiesFactory.h>
#include <Engine.h>
#import <UIKit/UIView.h>

template<>
class Engine<UBE_IOS> : public BaseEngine {
  UIView *IosView;
public:
  Engine<UBE_IOS>(UIView *view) : IosView(view) {}
  bool Init() {}
}

#endif
