#ifndef __OBJLOADER_MATERIALLOADER_H__
#define __OBJLOADER_MATERIALLOADER_H__
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <Mesh.h>
#include <Traits.h>

//using namespace std;
/*
  This class parses a material file.
*/
namespace ObjLoader {

class MaterialLoader {
  enum Tokens { DIFFUSE, AMBIENT, MAP_DIFFUSE, NEW_MAT, SPECULAR, NS, NI, 
                END, UNKN };
  typedef std::map<std::string, Uberngine::Material*> NameToMaterialMap;
  typedef NameToMaterialMap::iterator NameToMaterialMapIt;
  typedef Uberngine::Mesh<Uberngine::Traits::MeshVertexType, Uberngine::Traits::MeshIDXType> MeshTy;

  std::vector<std::string> Errors;
  NameToMaterialMap MaterialMap;
  Uberngine::Material *CurrentMaterial;
  MeshTy::TextureList *TexList;
  //string CurrMatName;
  std::ifstream MaterialFile;
  std::string TokString;
  std::string ReadData;
  std::string DirectoryPath;
  Tokens readToken();
  bool parseDiffuse();
  bool parseAmbient();
  bool parseDiffuseMap();
  bool parseNewMaterial();
  bool parseSpecular();
  bool parseNs();
  bool parseNi();
public:

  MaterialLoader(const std::string &dir_path) : DirectoryPath(dir_path) {}
  //Parses a material file and adds each material found to its own map
  bool loadMaterialFile(const std::string &filename, MeshTy::TextureList *tlist, bool record_errors);
  //Retrieves a material from its name
  Uberngine::Material* getMaterial(const std::string& name) {
    NameToMaterialMapIt It = MaterialMap.find(name);
    if (It != MaterialMap.end())
      return It->second;

    return NULL;
  }

  const std::vector<std::string>& getErrors() const { return Errors; }

  ~MaterialLoader();
};

}

#endif
