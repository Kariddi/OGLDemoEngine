#include "Loader.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <set>

//#define REGISTER_ERROR(message) if (record_errors && !no_error) \
//                                  Errors.push_back(message + "\n");

using namespace std;
using namespace ObjLoader;
using namespace Uberngine;

const unsigned char Loader::PadAmount[] = { 0, 3, 2, 1 };

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
  ParseDataTy vertex_data;
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
  ParseDataTy vertex_data;
  stringstream ss(ReadData);
  ss >> skipws >> vertex_data;
  TextureData->push_back(vertex_data);
  ss >> skipws >> vertex_data;
  TextureData->push_back(vertex_data);
  return !ss.fail();
}

bool Loader::parseNormalVert() {
  ParseDataTy vertex_data;
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
  ParseIndexTy index;
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
 
void Loader::removeRedundantVertices(unsigned int *indices) {

  ParseIndexTy VerticesNum = CurrentIndex;
  DataVector &Vertices = *FinalVertices;
  ParseDataTy min = std::numeric_limits<ParseDataTy>::min(), max = std::numeric_limits<ParseDataTy>::max();
  //Compute classes for vertices , a class is the sum of the position components.
  for (int i = 0; i < VerticesNum; ++i) {
    ParseIndexTy Index = i*Stride;
    ParseDataTy VClass = Vertices[Index] + Vertices[Index+1] + Vertices[Index+2];
    if (VClass < min)
      min = VClass;
    if (VClass > max)
      max = VClass;
  }
  ParseDataTy threshold = (max - min) / VerticesNum / 10;
  ParseDataTy MultFact = 1 / threshold / 10;
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

  for (ParseIndexTy i = 0; i < VerticesNum; ++i) {
    unsigned int IVClass = computeVertexClass(i*Stride, Offset, MultFact);
    ClassMap[IVClass].push_back(i);
  }
  std::cout << "Max Original Idx: " << *std::max_element(indices, indices + CurrentIndex) << std::endl;
  //Verify specifically each class
  for (unsigned int i = 0 ; i < BucketNum; ++i) {
    IndexVector &CurrClass = ClassMap[i];
    if (CurrClass.empty())
      continue;
    
    for (IndexVectorIt I = CurrClass.begin(), E = CurrClass.end(); I != E-1; ++I)
      for (IndexVectorIt I2 = I+1; I2 != E; ++I2) {
        ParseIndexTy IDX1 = (*I)*Stride;
        ParseIndexTy IDX2 = (*I2)*Stride;
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
  ParseIndexTy *OrderedIndices = new ParseIndexTy[CurrentIndex];
  std::copy(indices, indices + CurrentIndex, OrderedIndices);
  std::sort(OrderedIndices, OrderedIndices + CurrentIndex);
  ParseIndexTy *NewEnd = std::unique(OrderedIndices, OrderedIndices + CurrentIndex);
  ParseIndexTy Size = NewEnd - OrderedIndices;
  DataVector *NewFinalVertices = new DataVector(Size*Stride);
  //Clean up the vertices and correct the indices
  for (ParseIndexTy i = 1; i < CurrentIndex; ++i) {
    ParseIndexTy *ReturnedIndex = std::lower_bound(OrderedIndices, NewEnd, indices[i]);
    ParseIndexTy Pos = ReturnedIndex - OrderedIndices;
    indices[i] = Pos;
    //Vertex not removed, copy to final vertex
  }
  for (ParseIndexTy i = 0; i < Size; ++i) {
    DataVectorIt StartIt = FinalVertices->begin() + *(OrderedIndices+i)*Stride;
    std::copy(StartIt, StartIt+Stride, NewFinalVertices->begin() + i*Stride);
  }
  delete [] OrderedIndices;
  delete FinalVertices;
  FinalVertices = NewFinalVertices;

  std::cout << "Original Vertex num: " << VerticesNum << std::endl;
  std::cout << "Final Vertices size: " << FinalVertices->size()/Stride << std::endl;
  std::cout << "Max index: " << FinalVertices->size()/Stride << std::endl;
  std::cout << "True Max Index: " << *std::max_element(indices, indices + CurrentIndex) << std::endl; 
  std::cout << "Threshold: " << threshold << std::endl;
  std::cout << "Max: " << max << std::endl;
  std::cout << "Min: " << min << std::endl;
}

Mesh *Loader::loadMesh(const std::string& filename, bool record_errors) {

  return loadMesh<UBE_LOADER_FLOAT, UBE_LOADER_FLOAT>(filename, record_errors);
}


/*
Loader::~Loader() {
  for (NameToMatLoaderMapIt I = MatLoadersMap.begin(), 
       E = MatLoadersMap.end(); I != E; ++I) {
    delete I->second;
  } 
}*/
