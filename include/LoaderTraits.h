#ifndef __UBERNGINE_LOADERTRAITS_H__
#define __UBERNGINE_LOADERTRAITS_H__

namespace Uberngine {

enum ElementType { UBE_LOADER_UBYTE, UBE_LOADER_USHORT, UBE_LOADER_BYTE, UBE_LOADER_SHORT, UBE_LOADER_FLOAT };

template<int size>
struct LoaderElement {};

template<>
struct LoaderElement<UBE_LOADER_UBYTE> {
  typedef uint8_t Type;
  static const uint8_t Size = sizeof(uint8_t);
};

template<>
struct LoaderElement<UBE_LOADER_USHORT> {
  typedef uint16_t Type;
  static const uint8_t Size = sizeof(uint16_t);
};

template<>
struct LoaderElement<UBE_LOADER_BYTE> {
  typedef int8_t Type;
  static const uint8_t Size = sizeof(int8_t);
};

template<>
struct LoaderElement<UBE_LOADER_SHORT> {
  typedef int16_t Type;
  static const uint8_t Size = sizeof(int16_t);
};

template<>
struct LoaderElement<UBE_LOADER_FLOAT> {
  typedef float Type;
  static const uint8_t Size = sizeof(float);
};

}
#endif
