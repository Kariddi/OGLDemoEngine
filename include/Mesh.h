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

template<typename T>
struct Part {
  Material Mat;
  T *Indices;
  unsigned int IndicesSize;
  Part(T *ind, unsigned int ind_size, const Material *mat) : Mat(*mat), Indices(ind), 
                                                       IndicesSize(ind_size) {}
  ~Part() {
    if (Indices)
      delete [] Indices;
  }
};

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
  std::vector<Part<V>*> Parts;
  std::vector<Texture*> Textures;
  Mesh(T *verts, unsigned int vert_num, bool has_tex, bool has_norms, const std::vector<Part<V>*>& parts, const TextureList &tex) : Vertices(verts), 
         HasTexture(has_tex), HasNormals(has_norms), VerticesNum(vert_num), Parts(parts), Textures(tex) {}
  ~Mesh() {
    for (PartListIt I = Parts.begin(), E = Parts.end(); I != E; ++I)
      delete *I;
    for (TextureListIt I = Textures.begin(), E = Textures.end(); I != E; ++I)
      delete *I;
    if (Vertices)
      delete [] Vertices;
  }
};

}

#endif
