#include "Loader.h"
#include <sstream>
#include <iostream>
#include <cfloat>
#include <cmath>
#include <algorithm>
#include <set>

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
    if (PartsVector->size() > 0)
      PartsVector->back().MaxIndex = CurrentIndex-1; 
    CurrentPart.MaxIndex = 0;
    PartsVector->push_back(CurrentPart); 
  } else
    CurrPartSize = c;

  while (!ss.eof()) {
    char skip;
    ss >> index;
    --index;
    ++CurrentIndex;
    //CurrentPart.Indices->push_back(CurrentIndex++); 
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
    if (PartsVector->size() > 0)
      PartsVector->back().MaxIndex = CurrentIndex-1;
    CurrentPart.Mat = It->second->getMaterial(ReadData);
    CurrentPart.MaxIndex = 0;
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
/*
template<typename T, typename ValTy>
unsigned int Loader::binaryFind(T start, T end, const ValTy &Val) {
  int size = end - start;
  int middle = size / 2;
  if (*(start+middle) == 
  return 0;
}*/
 
void Loader::removeRedundantVertices(Traits::MeshIDXType *indices) {

  int VerticesNum = CurrentIndex;
  DataVector &Vertices = *FinalVertices;
  float min = FLT_MAX, max = FLT_MIN;
  //Compute classes for vertices , a class is the sum of the position components.
  for (int i = 0; i < VerticesNum; ++i) {
    int Index = i*Stride;
    float VClass = Vertices[Index] + Vertices[Index+1] + Vertices[Index+2];
    if (VClass < min)
      min = VClass;
    if (VClass > max)
      max = VClass;
  }
  float threshold = (max - min) / VerticesNum / 10;
  float MultFact = 1 / threshold;
  int Min = static_cast<int>(min*MultFact);
  int Max = static_cast<int>(max*MultFact);
  //Numerical error
  if ((min > 0.0001f || min < -0.0001f) && Min == 0)
    return;
  if ((max > 0.0001f || max < -0.0001f) && Max == 0)
    return;
 
  unsigned int BucketNum = static_cast<unsigned int>((Max - Min) + 1);
  unsigned int Offset = static_cast<unsigned int>((Min < 0) ? -Min : 0);
  IndexVector *ClassMap = new IndexVector[BucketNum];

  for (int i = 0; i < VerticesNum; ++i) {
    int IVClass = computeVertexClass(i*Stride, Offset, MultFact);
    ClassMap[IVClass].push_back(i);
  }
  //Verify specifically each class
  for (int i = 0 ; i < BucketNum; ++i) {
    IndexVector &CurrClass = ClassMap[i];
    if (CurrClass.empty())
      continue;
    
    for (IndexVectorIt I = CurrClass.begin(), E = CurrClass.end(); I != E-1; ++I)
      for (IndexVectorIt I2 = I+1; I2 != E; ++I2) {
        Traits::MeshIDXType IDX1 = (*I)*Stride;
        Traits::MeshIDXType IDX2 = (*I2)*Stride;
        if (IDX1 == IDX2)
          continue;
        int norm_idx_off = (HasTex) ? 5 : 3;
        bool equal = true;
        //Check Vertex Position Data
        if (!(fabsf(Vertices[IDX1] - Vertices[IDX2]) < threshold &&
            fabsf(Vertices[IDX1+1] - Vertices[IDX2+1]) < threshold &&
            fabsf(Vertices[IDX1+2] - Vertices[IDX2+2]) < threshold))
          equal = false;
        if (HasTex && equal && !(fabsf(Vertices[IDX1+3] - Vertices[IDX2+3]) < threshold &&
            fabsf(Vertices[IDX1+4] - Vertices[IDX2+4]) < threshold))
          equal = false;
        if (HasNorm && equal && !(fabsf(Vertices[IDX1+norm_idx_off] - Vertices[IDX2+norm_idx_off]) < threshold &&
            fabsf(Vertices[IDX1+norm_idx_off+1] - Vertices[IDX2+norm_idx_off+1]) < threshold &&
            fabsf(Vertices[IDX1+norm_idx_off+2] - Vertices[IDX2+norm_idx_off+2]) < threshold))
          equal = false;
        if (equal) {
          indices[*I2] = *I;
          *I2 = *I;
          //std::cout << equal << std::endl;
        }
      }
  }
  delete [] ClassMap;
  Traits::MeshIDXType *OrderedIndices = new Traits::MeshIDXType[CurrentIndex];
  std::copy(indices, indices + CurrentIndex, OrderedIndices);
  std::sort(OrderedIndices, OrderedIndices + CurrentIndex);
  Traits::MeshIDXType *NewEnd = std::unique(OrderedIndices, OrderedIndices + CurrentIndex);
  unsigned int Size = NewEnd - OrderedIndices;
  DataVector *NewFinalVertices = new DataVector(Size*Stride);
  //Clean up the vertices and correct the indices
  for (int i = 1; i < CurrentIndex; ++i) {
    Traits::MeshIDXType *ReturnedIndex = std::lower_bound(OrderedIndices, NewEnd, indices[i]);
    unsigned int Pos = ReturnedIndex - OrderedIndices;
    indices[i] = Pos;
    //Vertex not removed, copy to final vertex
  }
  for (unsigned int i = 0; i < Size; ++i) {
    DataVectorIt StartIt = FinalVertices->begin() + *(OrderedIndices+i)*Stride;
    std::copy(StartIt, StartIt+Stride, NewFinalVertices->begin() + i*Stride);
  }
  delete [] OrderedIndices;
  delete FinalVertices;
  FinalVertices = NewFinalVertices;
/*
  std::cout << "Original Vertex num: " << VerticesNum*Stride << std::endl;
  std::cout << "Final Vertices size: " << FinalVertices->size() << std::endl;
  std::cout << "Max index: " << FinalVertices->size()/Stride << std::endl;
  std::cout << "True Max Index: " << *std::max_element(indices, indices + CurrentIndex) << std::endl; 
  std::cout << "Threshold: " << threshold << std::endl;
  std::cout << "Max: " << max << std::endl;
  std::cout << "Min: " << min << std::endl;*/
}

Loader::MeshTy *Loader::constructMesh() {
//  MeshTy *NewMesh;
  HasNorm = (NormalData->size() > 0);
  HasTex = (TextureData->size() > 0);
  delete VertexData;
  delete NormalData;
  delete TextureData;
  
  Stride = 3;
  if (HasNorm)
    Stride += 3;
  if (HasTex)
    Stride += 2;

  
  Traits::MeshIDXType *OptIndices = new Traits::MeshIDXType[CurrentIndex];
  for (int i = 0; i < CurrentIndex; ++i)
    OptIndices[i] = i;
  removeRedundantVertices(OptIndices);

  unsigned int VertexNum = FinalVertices->size();
  Traits::MeshVertexType *Vertices = new Traits::MeshVertexType[VertexNum];
  //Creating the final vertex vector
  std::copy(FinalVertices->begin(), FinalVertices->end(), Vertices);
  delete FinalVertices;
  std::vector<MeshTy::PartTy*> PartVect;
  //Creating the indices vectors for each part. 
  unsigned int PartIdx = 0;
  for (vector<Part>::iterator I = PartsVector->begin(), E = PartsVector->end(); I != E; ++I) {
    unsigned int IdxSize = I->MaxIndex;
    Traits::MeshIDXType *Indices = new Traits::MeshIDXType[IdxSize];
    for (int i = 0; PartIdx <= IdxSize; ++i, ++PartIdx)
      Indices[i] = OptIndices[PartIdx];
    //std::copy(I->Indices->begin(), I->Indices->end(), Indices);
    //delete I->Indices;
    MeshTy::PartTy *NewPart = new MeshTy::PartTy(Indices, IdxSize+1, I->Mat);
    PartVect.push_back(NewPart);
  }
  delete PartsVector;
  delete [] OptIndices;
  //Creating the new Mesh
  MeshTy *NewMesh = new MeshTy(Vertices, VertexNum, HasTex, HasNorm, PartVect, TexList);
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
  if (PartsVector->size() > 0)
    PartsVector->back().MaxIndex = CurrentIndex-1;
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
