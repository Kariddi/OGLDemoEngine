#ifndef __LOADER_OBJMATERIALLOADER_H__
#define __LOADER_OBJMATERIALLOADER_H__
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <Mesh.h>

//using namespace std;
/*
  This class parses a material file.
*/
namespace Loader {

class ObjMaterialLoader {
  enum Tokens { DIFFUSE, AMBIENT, MAP_DIFFUSE, NEW_MAT, SPECULAR, NS, NI, 
                END, UNKN };
  typedef std::map<std::string, Uberngine::Material*> NameToMaterialMap;
  typedef NameToMaterialMap::iterator NameToMaterialMapIt;
//  typedef Uberngine::Mesh<Uberngine::Traits::MeshVertexType, Uberngine::Traits::MeshIDXType> MeshTy;

  std::vector<std::string> Errors;
  NameToMaterialMap MaterialMap;
  Uberngine::Material *CurrentMaterial;
  Uberngine::Mesh::TextureDataList *TexList;
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

  ObjMaterialLoader(const std::string &dir_path) : DirectoryPath(dir_path) {}
  //Parses a material file and adds each material found to its own map
  bool loadMaterialFile(const std::string &filename, Uberngine::Mesh::TextureDataList *tlist, bool record_errors);
  //Retrieves a material from its name
  Uberngine::Material* getMaterial(const std::string& name) {
    NameToMaterialMapIt It = MaterialMap.find(name);
    if (It != MaterialMap.end())
      return It->second;

    return NULL;
  }

  const std::vector<std::string>& getErrors() const { return Errors; }

  ~ObjMaterialLoader();
};

}

#endif
