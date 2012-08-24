#include "Loader.h"
#include <sstream>
#include <iostream>
#include <algorithm>

#define REGISTER_ERROR(message) if (record_errors && !no_error) \
                                  Errors.push_back(message + "\n");

using namespace std;
using namespace ObjLoader;
using namespace Uberngine;

Loader::Tokens Loader::readToken() {
  char skip;
  //Skip empty lines
  while ((skip = MeshFile.get()) == '\n');

  //If we are done return the end token
  if (MeshFile.eof())
    return END;

  MeshFile.putback(skip);
  //Read the token string and the data associated
  getline(MeshFile, TokString, ' ');
  getline(MeshFile, ReadData, '\n');

/*  switch ( TokString[0] ) {
  case 'v':
    if (TokString[1] == '\0')
      ToReturn = VERT;
    else if (TokString[1] == 't')
      ToReturn = VERT_TEX;
    else if (TokString[1] == 'n')
      ToReturn = VERT_NORM;
    break;
  case 'f':
    ToReturn = FACE;
    break;
  case 'm':
    ToReturn = MTR_LIB;
    break;
  case 'u':
    ToReturn = MTR;
    break;

  }*/
  //Parse the token
  if (TokString.compare("v") == 0)
    return VERT;
  else if (TokString.compare("vt") == 0)
    return VERT_TEX;
  else if (TokString.compare("vn") == 0)
    return VERT_NORM;
  else if (TokString.compare("f") == 0)
    return FACE;
  else if (TokString.compare("mtllib") == 0)
    return MTR_LIB;
  else if (TokString.compare("usemtl") == 0)
    return MTR;
  
  return UNKN;
}

bool Loader::parseVertex() { 
  float vertex_data;
  stringstream ss(ReadData);
  ss >> skipws >> vertex_data;
  VertexData->push_back(vertex_data);
  ss >> skipws >> vertex_data;
  VertexData->push_back(vertex_data);
  ss >> skipws >> vertex_data;
  VertexData->push_back(vertex_data);
  return !ss.fail();
}

bool Loader::parseTextureVert() {
  float vertex_data;
  stringstream ss(ReadData);
  ss >> skipws >> vertex_data;
  TextureData->push_back(vertex_data);
  ss >> skipws >> vertex_data;
  TextureData->push_back(vertex_data);
  return !ss.fail();
}

bool Loader::parseNormalVert() {
  float vertex_data;
  stringstream ss(ReadData);
  ss >> skipws >> vertex_data;
  NormalData->push_back(vertex_data);
  ss >> skipws >> vertex_data;
  NormalData->push_back(vertex_data);
  ss >> skipws >> vertex_data;
  NormalData->push_back(vertex_data);
  return !ss.fail();
}

bool Loader::parseFace() {
  stringstream ss(ReadData);
  Uberngine::Traits::MeshIDXType index;
//  size_t pos = 1;
  int c = count(ReadData.begin(), ReadData.end(), ' ');
  //Supported only triangles
  if (c != 2)
    return false;
  if (CurrPartSize != 0 && CurrPartSize != c) {
    CurrPartSize = c;
    CurrentPart.Indices = new IndexVector();
    PartsVector->push_back(CurrentPart); 
  } else
    CurrPartSize = c;

  while (!ss.eof()) {
    char skip;
    ss >> index;
    --index;
    CurrentPart.Indices->push_back(CurrentIndex++); 
    FinalVertices->push_back(VertexData->at(3*index));
    FinalVertices->push_back(VertexData->at(3*index+1));
    FinalVertices->push_back(VertexData->at(3*index+2));
    skip = ss.get();
    if (skip == ' ' || ss.eof())
      continue;
    else if (skip != '/')
      return false;
    if (ss.peek() != '/') {
      ss >> index;
      --index;
      FinalVertices->push_back(TextureData->at(2*index));
      FinalVertices->push_back(TextureData->at(2*index+1));
    }
    skip = ss.get();
    if (skip == ' ' || ss.eof())
      continue;
    else if (skip != '/')
      return false;
    if (ss.peek() != '/') {
      ss >> index;
      --index;
      FinalVertices->push_back(NormalData->at(3*index));
      FinalVertices->push_back(NormalData->at(3*index+1));
      FinalVertices->push_back(NormalData->at(3*index+2));
    }
  }
  
  return true;
}

bool Loader::parseMaterial() {
    CurrPartSize = 0;
    NameToMatLoaderMapIt It = MatLoadersMap.find(CurrentMatLib);
    if (It == MatLoadersMap.end())
      return false;
    CurrentPart.Mat = It->second->getMaterial(ReadData);
    CurrentPart.Indices = new IndexVector();
    PartsVector->push_back(CurrentPart);
    
    return true;
}

bool Loader::parseMatLib() { 
  MaterialLoader* NewMatLoad = new MaterialLoader(DirectoryPath);
  MatLoadersMap[ReadData] = NewMatLoad;
  CurrentMatLib = ReadData;
  bool ret_val = NewMatLoad->loadMaterialFile(ReadData, &TexList, 
                                              RecordErrors);
  Errors.insert(Errors.end(), NewMatLoad->getErrors().begin(), 
                NewMatLoad->getErrors().end());
  return ret_val;
}


Loader::MeshTy *Loader::constructMesh() {
//  MeshTy *NewMesh;
  bool has_norm = (NormalData->size() > 0);
  bool has_tex = (TextureData->size() > 0);
  delete VertexData;
  delete NormalData;
  delete TextureData;

  Traits::MeshVertexType *Vertices = new Traits::MeshVertexType[FinalVertices->size()];
  //Creating the final vertex vector
  std::copy(FinalVertices->begin(), FinalVertices->end(), Vertices);
  delete FinalVertices;
  std::vector<MeshTy::PartTy*> PartVect;
  //Creating the indices vectors for each part. 
  for (vector<Part>::iterator I = PartsVector->begin(), E = PartsVector->end(); I != E; ++I) {
    unsigned int IdxSize = I->Indices->size();
    Traits::MeshIDXType *Indices = new Traits::MeshIDXType[IdxSize];
    std::copy(I->Indices->begin(), I->Indices->end(), Indices);
    delete I->Indices;
    MeshTy::PartTy *NewPart = new MeshTy::PartTy(Indices, IdxSize, I->Mat);
    PartVect.push_back(NewPart);
  }
  delete PartsVector;
  //Creating the new Mesh
  MeshTy *NewMesh = new MeshTy(Vertices, CurrentIndex, has_tex, has_norm, PartVect, TexList);
  TexList.clear();

  return NewMesh;
}

Loader::MeshTy *Loader::loadMesh(const string& filename, bool record_errors) {
  VertexData = new DataVector();
  NormalData = new DataVector();
  TextureData = new DataVector();
  PartsVector = new vector<Part>();
  FinalVertices = new DataVector();
  CurrentIndex = 0;

  RecordErrors = record_errors;

  string full_pathname = DirectoryPath + filename;
  MeshFile.open(full_pathname.c_str(), ifstream::in);
  if (!MeshFile.good()) {
    MeshFile.close();
    return NULL;
  }

  Tokens NextToken; 
  
  //Parsing loop. Retrieves the token , and dispatches execution to the appropriate function
  while ((NextToken = readToken()) != END) {
  
    switch (NextToken) {
    bool no_error;
    case VERT:
      no_error = parseVertex();
      REGISTER_ERROR("Vertex format error: " + ReadData)
      break;
    case VERT_TEX:
      no_error = parseTextureVert();
      REGISTER_ERROR("Texture vertex format error: " + ReadData)
      break;
    case VERT_NORM:
      no_error = parseNormalVert();
      REGISTER_ERROR("Vertex normal format error: " + ReadData)
      break;
    case FACE:
      no_error = parseFace();
      REGISTER_ERROR("Face format error: " + ReadData)
      break;
    case MTR:
      no_error = parseMaterial();
      REGISTER_ERROR("Material format error: " + ReadData)
      break;
    case MTR_LIB:
      no_error = parseMatLib();
      REGISTER_ERROR("Material lib format error: " + ReadData)
      break;
    default:
      if (record_errors)
        Errors.push_back("Not Recognized: TOK " + TokString +
                         " with DATA " + ReadData + "\n");
      break;
    }
  
  }
  MeshFile.close();
/*
  delete VertexData;
  delete NormalData;
  delete TextureData;*/
  //delete PartsVector;
//  delete FinalVertices;

  return constructMesh();
}
/*
Loader::~Loader() {
  for (NameToMatLoaderMapIt I = MatLoadersMap.begin(), 
       E = MatLoadersMap.end(); I != E; ++I) {
    delete I->second;
  } 
}*/
