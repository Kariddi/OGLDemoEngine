#ifndef __UBERNGINE_LOADERTRAITS_H__
#define __UBERNGINE_LOADERTRAITS_H__

#include <Defs.h>

namespace Uberngine {

enum ElementType { UBE_LOADER_NONE, UBE_LOADER_UBYTE, UBE_LOADER_USHORT, UBE_LOADER_BYTE, UBE_LOADER_SHORT, UBE_LOADER_FLOAT };

template<ElementType size>
struct LoaderElement {};

template<>
struct LoaderElement<UBE_LOADER_UBYTE> {
  typedef uint8_t Type;
  static const uint8_t Size = sizeof(uint8_t);
  static const bool Normalize = true;
};

template<>
struct LoaderElement<UBE_LOADER_USHORT> {
  typedef uint16_t Type;
  static const uint8_t Size = sizeof(uint16_t);
  static const bool Normalize = true;
};

template<>
struct LoaderElement<UBE_LOADER_BYTE> {
  typedef int8_t Type;
  static const uint8_t Size = sizeof(int8_t);
  static const bool Normalize = true;
};

template<>
struct LoaderElement<UBE_LOADER_SHORT> {
  typedef int16_t Type;
  static const uint8_t Size = sizeof(int16_t);
  static const bool Normalize = true;
};

template<>
struct LoaderElement<UBE_LOADER_FLOAT> {
  typedef float Type;
  static const uint8_t Size = sizeof(float);
  static const bool Normalize = false;
};

//For 4-byte alignment of data ...
template<int modulo_size>
struct Padding {};

template<>
struct Padding<0> {
  static const uint8_t PadAmount = 0;
};

template<>
struct Padding<1> {
  static const uint8_t PadAmount = 3;
};

template<>
struct Padding<2> {
  static const uint8_t PadAmount = 2;
};

template<>
struct Padding<3> {
  static const uint8_t PadAmount = 1;
};

template<bool has_tex, bool has_norm>
struct VertexUtils {
  template<typename T, typename VType>
  inline static void CopyPosData(VType &vert, T data, uint8_t pos) { vert.Position.Data[pos] = data; }
  template<typename T, typename VType>
  inline static void CopyTexData(VType &vert, T data, uint8_t pos) { vert.TextureCoord.Data[pos] = data; }
  template<typename T, typename VType>
  inline static void CopyNormData(VType &vert, T data, uint8_t pos) { vert.Normal.Data[pos] = data; }
  template<typename VType>
  inline static int GetTexOffset() { return offsetof(VType, TextureCoord); }
  template<typename VType>
  inline static int GetNormOffset() { return offsetof(VType, Normal); }
};

template<>
struct VertexUtils<false, true> {
  template<typename T, typename VType>
  inline static void CopyPosData(VType &vert, T data, uint8_t pos) { vert.Position.Data[pos] = data; }
  template<typename T, typename VType>
  inline static void CopyTexData(VType &vert, T data, uint8_t pos) {}
  template<typename T, typename VType>
  inline static void CopyNormData(VType &vert, T data, uint8_t pos) { vert.Normal.Data[pos] = data; }
  template<typename VType>
  inline static int GetTexOffset() { return 0; }
  template<typename VType>
  inline static int GetNormOffset() { return offsetof(VType, Normal); }
};

template<>
struct VertexUtils<true, false> {
  template<typename T, typename VType>
  inline static void CopyPosData(VType &vert, T data, uint8_t pos) { vert.Position.Data[pos] = data; }
  template<typename T, typename VType>
  inline static void CopyTexData(VType &vert, T data, uint8_t pos) { vert.TextureCoord.Data[pos] = data; }
  template<typename T, typename VType>
  inline static void CopyNormData(VType &vert, T data, uint8_t pos) {}
  template<typename VType>
  inline static int GetTexOffset() { return offsetof(VType, TextureCoord); }
  template<typename VType>
  inline static int GetNormOffset() { return 0; }
};

template<>
struct VertexUtils<false, false> {
  template<typename T, typename VType>
  inline static void CopyPosData(VType &vert, T data, uint8_t pos) { vert.Position.Data[pos] = data; }
  template<typename T, typename VType>
  inline static void CopyTexData(VType &vert, T data, uint8_t pos) {}
  template<typename T, typename VType>
  inline static void CopyNormData(VType &vert, T data, uint8_t pos) {}
  template<typename VType>
  inline static int GetTexOffset() { return 0; }
  template<typename VType>
  inline static int GetNormOffset() { return 0; }
};

//Padded attribute array to align vertex data to 4-byte boundaries
template<typename T, int size>
union AttributeArray {
  T Data[size];
  uint8_t Pad[sizeof(T)*size + Padding<sizeof(T)*size % 4>::PadAmount]; 
};

template<ElementType Texture, ElementType Normals>
struct VertexData {
typedef LoaderElement<UBE_LOADER_FLOAT> PosElementInfo;
typedef LoaderElement<Texture> TexElementInfo;
typedef LoaderElement<Normals> NormElementInfo;
  static const uint8_t Stride = 8;
  static const bool HasNorm = true;
  static const bool HasTex = true;
  AttributeArray<typename PosElementInfo::Type, 3> Position;
  AttributeArray<typename TexElementInfo::Type, 2> TextureCoord;
  AttributeArray<typename NormElementInfo::Type, 3> Normal;
};

template<ElementType Normals>
struct VertexData<UBE_LOADER_NONE, Normals> {
typedef LoaderElement<UBE_LOADER_FLOAT> PosElementInfo;
typedef LoaderElement<Normals> NormElementInfo;
  static const uint8_t Stride = 6;
  static const bool HasNorm = true;
  static const bool HasTex = false; 
  AttributeArray<typename PosElementInfo::Type, 3> Position;
  AttributeArray<typename NormElementInfo::Type, 3> Normal;
};

template<ElementType Texture>
struct VertexData<Texture, UBE_LOADER_NONE> {
typedef LoaderElement<UBE_LOADER_FLOAT> PosElementInfo;
typedef LoaderElement<Texture> TexElementInfo;
  static const uint8_t Stride = 5;
  static const bool HasNorm = false;
  static const bool HasTex = true; 
  AttributeArray<typename PosElementInfo::Type, 3> Position;
  AttributeArray<typename TexElementInfo::Type, 2> TextureCoord;
};

template<>
struct VertexData<UBE_LOADER_NONE, UBE_LOADER_NONE> {
typedef LoaderElement<UBE_LOADER_FLOAT> PosElementInfo;
  static const uint8_t Stride = 3;
  static const bool HasNorm = false;
  static const bool HasTex = false; 
  AttributeArray<typename PosElementInfo::Type, 3> Position;
};

/*
template<ElementType Texture, ElementType Normal>
union PaddedVertexData {
typedef VertexData<Texture, Normal> VData;
  VData Vertex;
  uint8_t Pad[sizeof(VData) + Padding<sizeof(VData) % 4>::PadAmount];
};
*/
}
#endif
