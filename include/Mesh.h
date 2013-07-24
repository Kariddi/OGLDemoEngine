#ifndef __UBERNGINE_MESH_H__
#define __UBERNGINE_MESH_H__
#include <vector>
#include <TextureData.h>
#include <LoaderTraits.h>
/*
  These structs contain the Mesh and material data
*/

namespace Uberngine {

struct Material {
  typedef std::vector<unsigned short> TextureDataIdxList;
  float KAr, KAg, KAb;
  float KDr, KDg, KDb;
  float KSr, KSg, KSb;
  float Ns, Ni;
  //TextureDatas
  TextureDataIdxList TextureIdx;
  Material() {}
};

struct Part {
  Material Mat;
  char *Indices;
  unsigned int IndicesSize;
  //OGL specific
  unsigned char IdxElementSize;
  Part(void *ind, unsigned char idx_elem_size, 
       unsigned int ind_size, const Material *mat) : Mat(*mat), Indices(static_cast<char*>(ind)), 
                                                     IndicesSize(ind_size), IdxElementSize(idx_elem_size) {}
  ~Part() {
    if (Indices)
      delete [] Indices;
  }
};

/*
template<typename T>
struct Part {
  Material Mat;
  T *Indices;
  unsigned int IndicesSize;
  
  Part(T *ind, unsigned int ind_size, const Material *mat) : Mat(*mat), 
                                                             Indices(ind), IndicesSize(ind_size) {}
  ~Part() {
    if (Indices)
      delete [] Indices;
  }
};*/


struct AttributeInfo {
  unsigned short VertexStride;
  unsigned char NormSize;
  unsigned char TexSize;
  unsigned char NormOffset;
  unsigned char TexOffset;
  bool NormSigned;
  bool TexSigned;
  bool HasTexture;
  bool HasNormals;

  AttributeInfo() : VertexStride(0), NormSize(0), 
                    TexSize(0), NormOffset(0), TexOffset(0),
                    NormSigned(0), TexSigned(0) {} 
  AttributeInfo(unsigned short stride, unsigned char norm_size, unsigned char tex_size, 
                unsigned char norm_offset, unsigned char tex_offset,
                bool norm_signed, bool tex_signed) : VertexStride(stride), NormSize(norm_size), 
                                                     TexSize(tex_size), NormOffset(norm_offset), TexOffset(tex_offset),
                                                     NormSigned(norm_signed), TexSigned(tex_signed) {
    HasTexture = tex_offset == 0 ? false : true;
    HasNormals = norm_offset == 0 ? false : true;
  }
};

struct Mesh {
  typedef std::vector<Part*> PartList;
  typedef PartList::iterator PartListIt;
  typedef std::vector<TextureData*> TextureDataList;
  typedef TextureDataList::iterator TextureDataListIt;

  AttributeInfo AttInfo;
  char *Vertices;
  unsigned int VerticesNum;
  std::vector<Part*> Parts;
  std::vector<TextureData*> Textures;
  Mesh(void *verts, unsigned int vert_num, const AttributeInfo &ai, 
       const std::vector<Part*>& parts, 
       const TextureDataList &tex) : AttInfo(ai), Vertices(static_cast<char*>(verts)),
                                 VerticesNum(vert_num), Parts(parts), Textures(tex) {
    /*const unsigned char PadAmount[] = { 0, 3, 2, 1 };
    VertexStride = 3*LoaderElement<UBE_LOADER_FLOAT>::Size;
    if (has_tex)
      VertexStride += 2*tex_elem_type.first;
    if (has_norms)
      VertexStride += 3*norm_elem_type.first;
    VertexStride += PadAmount[VertexStride % 4];*/
  }
  ~Mesh() {
    for (PartListIt I = Parts.begin(), E = Parts.end(); I != E; ++I)
      delete *I;
    for (TextureDataListIt I = Textures.begin(), E = Textures.end(); I != E; ++I)
      delete *I;
    if (Vertices)
      delete [] Vertices;
  }

};

/*
template<typename T, typename V>
struct Mesh {
  typedef Part<V> PartTy;
  typedef std::vector<Part<V>*> PartList;
  typedef typename PartList::iterator PartListIt;
  typedef std::vector<TextureData*> TextureList;
  typedef TextureDataList::iterator TextureListIt;

  T *Vertices;
  bool HasTextureData;
  bool HasNormals;
  unsigned int VerticesNum;
  unsigned short VertexStride;
  std::vector<Part<V>*> Parts;
  std::vector<TextureData*> Textures;
  Mesh(T *verts, unsigned int vert_num, bool has_tex, bool has_norms, 
       const std::vector<Part<V>*>& parts, const TextureDataList &tex) : Vertices(verts), HasTexture(has_tex), 
                                                                     HasNormals(has_norms), VerticesNum(vert_num), 
                                                                     Parts(parts), TextureDatas(tex) {
    VertexStride = 3*sizeof(T);
    if (HasTextureData)
      VertexStride += 2*sizeof(T);
    if (HasNormals)
      VertexStride += 3*sizeof(T);
  }
  ~Mesh() {
    for (PartListIt I = Parts.begin(), E = Parts.end(); I != E; ++I)
      delete *I;
    for (TextureDataListIt I = Textures.begin(), E = Textures.end(); I != E; ++I)
      delete *I;
    if (Vertices)
      delete [] Vertices;
  }
};*/

}

#endif
