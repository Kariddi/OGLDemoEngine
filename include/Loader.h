#ifndef __OBJLOADER_LOADER_H__
#define __OBJLOADER_LOADER_H__
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <Mesh.h>
#include <MaterialLoader.h>
#include <Traits.h>

//using namespace std;

namespace ObjLoader {

class MaterialLoader;

class Loader {
  enum Tokens { VERT, VERT_TEX, VERT_NORM, FACE, MTR_LIB, 
                MTR, END, UNKN};

  typedef std::map<std::string, MaterialLoader*> NameToMatLoaderMap;
  typedef NameToMatLoaderMap::iterator NameToMatLoaderMapIt;
  typedef std::vector<Uberngine::Traits::MeshVertexType> DataVector;
  typedef std::vector<Uberngine::Traits::MeshIDXType> IndexVector;
  typedef DataVector::iterator DataVectorIt;
  typedef IndexVector::iterator IndexVectorIt;
  typedef Uberngine::Mesh<Uberngine::Traits::MeshVertexType, Uberngine::Traits::MeshIDXType> MeshTy;
 
  struct Part {
    Uberngine::Material *Mat;
    //IndexVector *Indices;
    Uberngine::Traits::MeshIDXType MaxIndex;
  };

  std::vector<std::string> Errors;
  NameToMatLoaderMap MatLoadersMap;
  std::ifstream MeshFile;
  DataVector *VertexData;
  DataVector *NormalData;
  DataVector *TextureData;
  DataVector *FinalVertices;
  std::string ReadData;
  std::string TokString;
  std::string DirectoryPath;
  std::vector<Part> *PartsVector;
  MeshTy::TextureList TexList;
  Part CurrentPart;
  Uberngine::Traits::MeshIDXType CurrentIndex;
  std::string CurrentMatLib;
  bool RecordErrors;
  int CurrPartSize;
  bool HasNorm;
  bool HasTex;
  int Stride;

  //Retrieves the next token 
  Tokens readToken();
  bool parseVertex();
  bool parseTextureVert();
  bool parseNormalVert();
  bool parseFace();
  bool parseMaterial();
  bool parseMatLib();
  inline unsigned int computeVertexClass(unsigned int index, unsigned int offset, float mult_fact) {
    return (unsigned int)(((*FinalVertices)[index] + (*FinalVertices)[index+1] + 
                                      (*FinalVertices)[index+2])*mult_fact + offset);
  }
/* template<typename T, typename ValTy> 
 unsigned int binaryFind(T start, T end, const ValTy &Val); */
 void removeRedundantVertices(Uberngine::Traits::MeshIDXType *indices);
  MeshTy *constructMesh();

public:
  Loader(const std::string &dir_path) : DirectoryPath(dir_path) {}
  ~Loader() { clear(); }
  //Non-NULL on success
  MeshTy *loadMesh(const std::string& filename, bool record_errors);
  const std::vector<std::string>& getErrors() const { return Errors; }

  void clear() {  
    for (NameToMatLoaderMapIt I = MatLoadersMap.begin(), 
       E = MatLoadersMap.end(); I != E; ++I)
      delete I->second;
  }

  void clearErrors() { Errors.clear(); }
};

}

#endif
