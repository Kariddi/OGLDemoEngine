#ifndef __UBERNGINE_MESH_H__
#define __UBERNGINE_MESH_H__
#include <vector>
#include <Texture.h>
/*
  These structs contain the Mesh and material data
*/

namespace Uberngine {

struct Material {
  typedef std::vector<unsigned short> TextureIdxList;
  float KAr, KAg, KAb;
  float KDr, KDg, KDb;
  float KSr, KSg, KSb;
  float Ns, Ni;
  //Textures
  TextureIdxList TextureIdx;
  Material() {}
};

struct Part {
  Material Mat;
  char *Indices;
  unsigned int IndicesSize;
  //OGL specific
  unsigned char IdxElementSize;
  Part(void *ind, unsigned char idx_elem_size, 
       unsigned int ind_size, const Material *mat) : Mat(*mat), Indices(reinterpret_cast<char*>(ind)), 
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


struct Mesh {
  typedef std::vector<Part*> PartList;
  typedef PartList::iterator PartListIt;
  typedef std::vector<Texture*> TextureList;
  typedef TextureList::iterator TextureListIt;
  typedef std::pair<unsigned char, bool> SizeSignedPair;

  bool HasTexture;
  bool HasNormals;
  char *Vertices;
  unsigned int VerticesNum;
  unsigned short VertexStride;
  unsigned short IndexSize;
  std::vector<Part*> Parts;
  std::vector<Texture*> Textures;
  //OGL Specific
  SizeSignedPair NormElementType;
  SizeSignedPair TexElementType;
  Mesh(void *verts, unsigned int vert_num, bool has_tex, bool has_norms,
       SizeSignedPair norm_elem_type, SizeSignedPair tex_elem_type,
       const std::vector<Part*>& parts, const TextureList &tex) : HasTexture(has_tex), HasNormals(has_norms), Vertices(reinterpret_cast<char*>(verts)),
                                                                  VerticesNum(vert_num), 
                                                                  NormElementType(norm_elem_type), TexElementType(tex_elem_type), 
                                                                  Parts(parts), Textures(tex) {
    const unsigned char PadAmount[] = { 0, 3, 2, 1 };
    VertexStride = 3*sizeof(float);
    if (has_tex)
      VertexStride += 2*tex_elem_type.first;
    if (has_norms)
      VertexStride += 3*norm_elem_type.first;
    VertexStride += PadAmount[VertexStride % 4];
  }
  ~Mesh() {
    for (PartListIt I = Parts.begin(), E = Parts.end(); I != E; ++I)
      delete *I;
    for (TextureListIt I = Textures.begin(), E = Textures.end(); I != E; ++I)
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
  typedef std::vector<Texture*> TextureList;
  typedef TextureList::iterator TextureListIt;

  T *Vertices;
  bool HasTexture;
  bool HasNormals;
  unsigned int VerticesNum;
  unsigned short VertexStride;
  std::vector<Part<V>*> Parts;
  std::vector<Texture*> Textures;
  Mesh(T *verts, unsigned int vert_num, bool has_tex, bool has_norms, 
       const std::vector<Part<V>*>& parts, const TextureList &tex) : Vertices(verts), HasTexture(has_tex), 
                                                                     HasNormals(has_norms), VerticesNum(vert_num), 
                                                                     Parts(parts), Textures(tex) {
    VertexStride = 3*sizeof(T);
    if (HasTexture)
      VertexStride += 2*sizeof(T);
    if (HasNormals)
      VertexStride += 3*sizeof(T);
  }
  ~Mesh() {
    for (PartListIt I = Parts.begin(), E = Parts.end(); I != E; ++I)
      delete *I;
    for (TextureListIt I = Textures.begin(), E = Textures.end(); I != E; ++I)
      delete *I;
    if (Vertices)
      delete [] Vertices;
  }
};*/

}

#endif
