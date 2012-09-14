#include "ObjMaterialLoader.h"
#include <sstream>
#include <Texture.h>

#define REGISTER_ERROR(message) if (record_errors && !no_error) \
                                  Errors.push_back(message + "\n");


using namespace Loader;
using namespace std;

ObjMaterialLoader::Tokens ObjMaterialLoader::readToken() {
  //Skip empty lines
  char skip;
  while ((skip = MaterialFile.get()) == '\n');
  
  //If we are done , return the END token
  if (MaterialFile.eof())
    return END;

  MaterialFile.putback(skip);
  //Read the token string and the associated data
  getline(MaterialFile, TokString, ' ');
  getline(MaterialFile, ReadData, '\n');

  //Decode the token string and return the token
  if (TokString.compare("Ka") == 0)
    return AMBIENT;
  else if (TokString.compare("Kd") == 0)
    return DIFFUSE;
  else if (TokString.compare("map_Kd") == 0)
    return MAP_DIFFUSE;
  else if (TokString.compare("newmtl") == 0)
    return NEW_MAT;
  else if (TokString.compare("Ks") == 0)
    return SPECULAR;
  else if (TokString.compare("Ns") == 0)
    return NS;
  else if (TokString.compare("Ni") == 0)
    return NI; 
  else if(MaterialFile.eof())
    return END;
  
  return UNKN;

}

bool ObjMaterialLoader::parseDiffuse() {
  stringstream ss(ReadData);
  ss >> skipws >> CurrentMaterial->KDr;
  ss >> skipws >> CurrentMaterial->KDg;
  ss >> skipws >> CurrentMaterial->KDb;
  return !ss.fail();
}

bool ObjMaterialLoader::parseAmbient() {
  stringstream ss(ReadData);
  ss >> skipws >> CurrentMaterial->KAr;
  ss >> skipws >> CurrentMaterial->KAg;
  ss >> skipws >> CurrentMaterial->KAb;
  return !ss.fail();
}

bool ObjMaterialLoader::parseDiffuseMap() {
  string Extension = ReadData.substr(ReadData.find_last_of('.')+1);
  bool is_tga = false, is_png = false;
  //Only support TGA and PNG files for now
  if (Extension == "tga")
    is_tga = true;
  else if (Extension == "png")
    is_png = true;
  else
    return false;

  Texture *NewTexture = new Texture;

  if (is_tga && LoadTGA(NewTexture, (DirectoryPath + ReadData).c_str())) {
    TexList->push_back(NewTexture);
    CurrentMaterial->TextureIdx.push_back(TexList->size()-1);
    return true;
  }

  if (is_png && LoadPNG(NewTexture, (DirectoryPath + ReadData).c_str())) {
    TexList->push_back(NewTexture);
    CurrentMaterial->TextureIdx.push_back(TexList->size()-1);
    return true;
  }

  delete NewTexture;
  return false;  
  
}

bool ObjMaterialLoader::parseNewMaterial() {
  //CurrMatName = ReadData;
  CurrentMaterial = new Uberngine::Material;
  MaterialMap[ReadData] = CurrentMaterial;
  return true;
}

bool ObjMaterialLoader::parseSpecular() {
  stringstream ss(ReadData);
  ss >> skipws >> CurrentMaterial->KSr;
  ss >> skipws >> CurrentMaterial->KSg;
  ss >> skipws >> CurrentMaterial->KSb;
  return !ss.fail();
}

bool ObjMaterialLoader::parseNs() {
  stringstream ss(ReadData);
  ss >> skipws >> CurrentMaterial->Ns;
  return !ss.fail(); 
}

bool ObjMaterialLoader::parseNi() {
  stringstream ss(ReadData);
  ss >> skipws >> CurrentMaterial->Ni;
  return !ss.fail(); 
}

bool ObjMaterialLoader::loadMaterialFile(const string &filename, Uberngine::Mesh::TextureList *tlist, bool record_errors) {

 string full_pathname = DirectoryPath + filename;
 MaterialFile.open(full_pathname.c_str(), ifstream::in);

 if (!MaterialFile.good()) {
   MaterialFile.close();
   return false;
 }
 TexList = tlist;
 bool no_error = true;
 Tokens NextToken;
 //Parsing loop. Retrieves the next token and dispatches the execution to the appropriate function 
 while ((NextToken = readToken()) != END) {
   switch (NextToken) {
     case DIFFUSE:
       no_error = parseDiffuse();
       REGISTER_ERROR("Diffuse data format error: " + ReadData)
       break;
     case AMBIENT:
       no_error = parseAmbient();
       REGISTER_ERROR("Ambient data format error: " + ReadData)
       break;
     case NEW_MAT:
       no_error = parseNewMaterial();
       REGISTER_ERROR("New material data format error: " + ReadData)
       break;
     case SPECULAR:
       no_error = parseSpecular();
       REGISTER_ERROR("Specular data format error: " + ReadData)
       break;
     case MAP_DIFFUSE:
       no_error = parseDiffuseMap();
       REGISTER_ERROR("Diffuse Texture format error: " + ReadData)
       break;
     case NS:
       no_error = parseNs();
       REGISTER_ERROR("Ns data format error: " + ReadData)
       break;
     case NI:
       no_error = parseNi();
       REGISTER_ERROR("Ni data format error: " + ReadData)
       break; 
     default:
      if (record_errors)
        Errors.push_back("Not Recognized: TOK " + TokString +
                         " with DATA " + ReadData + "\n");
       break;
   }
 }

 MaterialFile.close(); 

 return true;
}

ObjMaterialLoader::~ObjMaterialLoader() {
  for (NameToMaterialMapIt I = MaterialMap.begin(), 
       E = MaterialMap.end(); I != E; ++I) {
    delete I->second;
  }
}
