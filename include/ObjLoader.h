#ifndef __LOADER_OBJLOADER_H__
#define __LOADER_OBJLOADER_H__
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>
#include <algorithm>
#include <Mesh.h>
#include <Defs.h>
#include <ObjMaterialLoader.h>
#include <LoaderTraits.h>

//using namespace std;
#define REGISTER_ERROR(message) if (record_errors && !no_error) \
                                  Errors.push_back(message + "\n");

namespace Loader {

template<Uberngine::ElementType Texture = Uberngine::UBE_LOADER_FLOAT, Uberngine::ElementType Normal = Uberngine::UBE_LOADER_FLOAT>
class ObjLoader {
  enum Tokens { VERT, VERT_TEX, VERT_NORM, FACE, MTR_LIB, 
                MTR, END, UNKN};

  typedef std::map<std::string, ObjMaterialLoader*> NameToMatLoaderMap;
  typedef NameToMatLoaderMap::iterator NameToMatLoaderMapIt;
  typedef Uberngine::LoaderElement<Uberngine::UBE_LOADER_FLOAT>::Type ParseDataTy;
  typedef uint32_t ParseIndexTy;
  typedef Uberngine::LoaderElement<Uberngine::UBE_LOADER_FLOAT> PosElementInfo;
  typedef Uberngine::LoaderElement<Texture> TexElementInfo;
  typedef Uberngine::LoaderElement<Normal> NormElementInfo;
  typedef typename PosElementInfo::Type PosDataType;
  typedef typename TexElementInfo::Type TexDataType;
  typedef typename NormElementInfo::Type NormDataType;
  typedef std::vector<PosDataType> PosVector;
  typedef std::vector<TexDataType> TexVector;
  typedef std::vector<NormDataType> NormVector;
  typedef std::vector<ParseDataTy> DataVector;
  typedef std::vector<ParseIndexTy> IndexVector;
  typedef DataVector::iterator DataVectorIt;
  typedef IndexVector::iterator IndexVectorIt;
//  typedef Uberngine::Mesh<Uberngine::Traits::MeshVertexType, Uberngine::Traits::MeshIDXType> MeshTy;
 
  struct LoaderPart {
    Uberngine::Material *Mat;
    ParseIndexTy MaxIndex;
  };

  std::vector<std::string> Errors;
  NameToMatLoaderMap MatLoadersMap;
  std::ifstream MeshFile;
  PosVector *PosData;
  NormVector *NormalData;
  TexVector *TextureData;
  IndexVector *FinalVertices;
  std::string ReadData;
  std::string TokString;
  std::string DirectoryPath;
  std::vector<LoaderPart> *PartsVector;
  Uberngine::Mesh::TextureList TexList;
  LoaderPart CurrentPart;
  ParseIndexTy CurrentIndex;
  std::string CurrentMatLib;
  bool RecordErrors;
  int CurrPartSize;
  bool HasNorm;
  bool HasTex;
  int NumAttribs;

  //Retrieves the next token 
  Tokens readToken();
  bool parseVertex();
  bool parseTextureVert();
  bool parseNormalVert();
  bool parseFace();
  bool parseMaterial();
  bool parseMatLib();
  inline unsigned int computeVertexClass(ParseIndexTy index, unsigned int offset, float mult_fact) {
    return (unsigned int)(((*PosData)[index] + (*PosData)[index+1] + 
                                      (*PosData)[index+2])*mult_fact + offset);
  }
/* template<typename T, typename ValTy> 
 unsigned int binaryFind(T start, T end, const ValTy &Val); */
  void removeRedundantVertices(ParseIndexTy *indices);
  
  template<typename T>
  Uberngine::Part *constructPart(T *Indices, ParseIndexTy *GlobalIndices, LoaderPart *Part, unsigned int Start, unsigned int End) {
    unsigned int IndicesSize = End - Start;
    for (unsigned int i = 0; Start < End; ++i, ++Start)
      Indices[i] = static_cast<T>(GlobalIndices[Start]);
    return new Uberngine::Part(Indices, sizeof(T), IndicesSize, Part->Mat);
  }

  template<Uberngine::ElementType Target>
  inline typename Uberngine::LoaderElement<Target>::Type normalizeData(ParseDataTy Val); 
  
  void copyAndNormalizeVertexData(char *TargetVertices);
  
  template<typename T>
  T *constructMeshWithVertexData(ParseIndexTy *opt_indices, Uberngine::AttributeInfo *ai);

  Uberngine::Mesh *constructMesh();

  void copyAndNormalizeVertexData();

public:
  ObjLoader(const std::string &dir_path) : DirectoryPath(dir_path) {}
  ~ObjLoader() { clear(); }
  //Non-NULL on success
  Uberngine::Mesh *loadMesh(const std::string& filename, bool record_errors);
  const std::vector<std::string>& getErrors() const { return Errors; }

  void clear() {  
    for (NameToMatLoaderMapIt I = MatLoadersMap.begin(), 
       E = MatLoadersMap.end(); I != E; ++I)
      delete I->second;
  }

  void clearErrors() { Errors.clear(); }
};

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal> template<Uberngine::ElementType Target>
typename Uberngine::LoaderElement<Target>::Type ObjLoader<Texture, Normal>::normalizeData(ParseDataTy Val) {
  typedef typename Uberngine::LoaderElement<Target>::Type T;
  const bool is_signed = std::numeric_limits<T>::is_signed;
  if (Uberngine::LoaderElement<Target>::Normalize) {
    T RetVal;
    if (is_signed)
      RetVal = static_cast<T>(Val >= 0 ? Val * std::numeric_limits<T>::max() : Val * -std::numeric_limits<T>::min());
    else
      RetVal = static_cast<T>(Val * std::numeric_limits<T>::max());

    return RetVal;
  } else {
    return Val;
  }
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal> template<typename T>
T *ObjLoader<Texture, Normal>::constructMeshWithVertexData(ParseIndexTy *opt_indices, Uberngine::AttributeInfo *ai) {
  typedef typename Uberngine::LoaderElement<Texture>::Type TextureTy;
  typedef typename Uberngine::LoaderElement<Normal>::Type NormalTy; 
  const uint8_t TSize = Uberngine::LoaderElement<Texture>::Size;
  const uint8_t NSize = Uberngine::LoaderElement<Normal>::Size;
  const bool TSign = std::numeric_limits<TextureTy>::is_signed;
  const bool NSign = std::numeric_limits<NormalTy>::is_signed;

  ParseIndexTy VertexNum = FinalVertices->size() / NumAttribs;
  T *Vertices = new T[VertexNum];
  for (ParseIndexTy i = 0; i < VertexNum; ++i) {
    ParseIndexTy Index = i*NumAttribs;
    ParseIndexTy VertIndex = FinalVertices->at(Index);
    int offset = 1;
    Vertices[i].Position.Data[0] = PosData->at(VertIndex);
    Vertices[i].Position.Data[1] = PosData->at(VertIndex+1);
    Vertices[i].Position.Data[2] = PosData->at(VertIndex+2);
    if (T::HasTex) {
      VertIndex = FinalVertices->at(Index+offset);
//      Vertices[i].Vertex.CopyTexData(TextureData->at(VertIndex), 0);
//      Vertices[i].Vertex.CopyTexData(TextureData->at(VertIndex+1), 1);
      Uberngine::VertexUtils<T::HasTex, T::HasNorm>::CopyTexData(Vertices[i], TextureData->at(VertIndex), 0);
      Uberngine::VertexUtils<T::HasTex, T::HasNorm>::CopyTexData(Vertices[i], TextureData->at(VertIndex+1), 1); 
      ++offset;
    }
    if (T::HasNorm) {
      VertIndex = FinalVertices->at(Index+offset);
      Uberngine::VertexUtils<T::HasTex, T::HasNorm>::CopyNormData(Vertices[i], NormalData->at(VertIndex), 0);
      Uberngine::VertexUtils<T::HasTex, T::HasNorm>::CopyNormData(Vertices[i], NormalData->at(VertIndex+1), 1);  
      Uberngine::VertexUtils<T::HasTex, T::HasNorm>::CopyNormData(Vertices[i], NormalData->at(VertIndex+2), 2);
    }
  }
  
  uint8_t NormalOffset = Uberngine::VertexUtils<T::HasTex, 
                                          T::HasNorm>::template GetNormOffset<T>();  
  uint8_t TextureOffset = Uberngine::VertexUtils<T::HasTex, 
                                          T::HasNorm>::template GetTexOffset<T>();
  uint8_t Stride = sizeof(T);
  Uberngine::AttributeInfo AI(Stride, NSize, TSize, NormalOffset, TextureOffset, NSign, TSign);
  *ai = AI;

  return Vertices;
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
Uberngine::Mesh *ObjLoader<Texture, Normal>::constructMesh() {
//  MeshTy *NewMesh;
  HasNorm = (NormalData->size() > 0);
  HasTex = (TextureData->size() > 0);
  NumAttribs = 1;
  if (HasNorm) ++NumAttribs;
  if (HasTex) ++NumAttribs;

  ParseIndexTy *OptIndices = new ParseIndexTy[CurrentIndex];
  for (ParseIndexTy i = 0; i < CurrentIndex; ++i)
    OptIndices[i] = i;
  removeRedundantVertices(OptIndices);

  ParseIndexTy VertexNum = FinalVertices->size() / NumAttribs;
  
  void *Vertices;
  Uberngine::AttributeInfo AI;
  if (HasNorm && HasTex)
    Vertices = constructMeshWithVertexData<Uberngine::VertexData<Texture, 
                                       Normal> >(OptIndices, &AI);
  else if (HasNorm)
    Vertices = constructMeshWithVertexData<Uberngine::VertexData<Uberngine::UBE_LOADER_NONE, 
                                       Normal> >(OptIndices, &AI);
  else if (HasTex)
    Vertices = constructMeshWithVertexData<Uberngine::VertexData<Texture, 
                                       Uberngine::UBE_LOADER_NONE> >(OptIndices, &AI);
  else
    Vertices = constructMeshWithVertexData<Uberngine::VertexData<Uberngine::UBE_LOADER_NONE, 
                                       Uberngine::UBE_LOADER_NONE> >(OptIndices, &AI);

  delete PosData;
  delete NormalData;
  delete TextureData;
  delete FinalVertices;

  std::vector<Uberngine::Part*> PartVect;
  //Creating the indices vectors for each part. 
  ParseIndexTy PartIdx = 0;
  for (typename std::vector<LoaderPart>::iterator I = PartsVector->begin(), E = PartsVector->end(); I != E; ++I) {
    ParseIndexTy IdxSize = I->MaxIndex;
    ParseIndexTy IndicesSize = IdxSize - PartIdx + 1;
    ParseIndexTy MaxPartIndex = *std::max_element(OptIndices + PartIdx, OptIndices + IdxSize + 1);
    uint8_t IdxBytes = 4;
    if ((MaxPartIndex & 0xFF) == MaxPartIndex)
      IdxBytes = 1;
    else if ((MaxPartIndex & 0xFFFF) == MaxPartIndex)
      IdxBytes = 2;

    void *Indices = new char[IndicesSize*IdxBytes];
    Uberngine::Part *NewPart = NULL;
    if (IdxBytes == 1)
      NewPart = constructPart(static_cast<uint8_t*>(Indices), OptIndices, &(*I), PartIdx, IdxSize+1);
    else if (IdxBytes == 2)
      NewPart = constructPart(static_cast<uint16_t*>(Indices), OptIndices, &(*I), PartIdx, IdxSize+1);
    else if (IdxBytes == 4)
      NewPart = constructPart(static_cast<uint32_t*>(Indices), OptIndices, &(*I), PartIdx, IdxSize+1);
    PartIdx = IdxSize+1;
    PartVect.push_back(NewPart);
  }
  delete PartsVector;
  delete [] OptIndices;

  //Creating the new Mesh
  Uberngine::Mesh *NewMesh = new Uberngine::Mesh(Vertices, VertexNum, AI, PartVect, TexList);
  TexList.clear();
  return NewMesh;

}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
Uberngine::Mesh *ObjLoader<Texture, Normal>::loadMesh(const std::string& filename, bool record_errors) {
  std::string full_pathname = DirectoryPath + filename;
  MeshFile.open(full_pathname.c_str(), std::ifstream::in);
  if (!MeshFile.good()) {
    MeshFile.close();
    return NULL;
  }

  //Initialization of Vectors
  PosData = new PosVector();
  NormalData = new NormVector();
  TextureData = new TexVector();
  PartsVector = new std::vector<LoaderPart>();
  FinalVertices = new IndexVector();
  //FinalVertices = new VertexVector();
  CurrentIndex = 0;

  //Don't make the vectors start empty
  PosData->reserve(10000);
  NormalData->reserve(10000);
  TextureData->reserve(10000);
  PartsVector->reserve(10000);
  FinalVertices->reserve(10000);

  RecordErrors = record_errors;


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

  return constructMesh();
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
typename ObjLoader<Texture,Normal>::Tokens ObjLoader<Texture, Normal>::readToken() {
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

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
bool ObjLoader<Texture, Normal>::parseVertex() { 
  ParseDataTy vertex_data;
  std::stringstream ss(ReadData);
  ss >> std::skipws >> vertex_data;
  PosData->push_back(vertex_data);
  ss >> std::skipws >> vertex_data;
  PosData->push_back(vertex_data);
  ss >> std::skipws >> vertex_data;
  PosData->push_back(vertex_data);
  return !ss.fail();
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
bool ObjLoader<Texture, Normal>::parseTextureVert() {
  ParseDataTy vertex_data;
  std::stringstream ss(ReadData);
  ss >> std::skipws >> vertex_data;
  TextureData->push_back(normalizeData<Texture>(vertex_data));
  ss >> std::skipws >> vertex_data;
  TextureData->push_back(normalizeData<Texture>(vertex_data));
  return !ss.fail();
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
bool ObjLoader<Texture, Normal>::parseNormalVert() {
  ParseDataTy vertex_data;
  std::stringstream ss(ReadData);
  ss >> std::skipws >> vertex_data;
  NormalData->push_back(normalizeData<Normal>(vertex_data));
  ss >> std::skipws >> vertex_data;
  NormalData->push_back(normalizeData<Normal>(vertex_data));
  ss >> std::skipws >> vertex_data;
  NormalData->push_back(normalizeData<Normal>(vertex_data));
  return !ss.fail();
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
bool ObjLoader<Texture, Normal>::parseFace() {
  std::stringstream ss(ReadData);
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
    FinalVertices->push_back(3*index);
    //CurrentPart.Indices->push_back(CurrentIndex++); 
    skip = ss.get();
    if (skip == ' ' || ss.eof())
      continue;
    else if (skip != '/')
      return false;
    if (ss.peek() != '/') {
      ss >> index;
      --index;
      FinalVertices->push_back(2*index);
    }
    skip = ss.get();
    if (skip == ' ' || ss.eof())
      continue;
    else if (skip != '/')
      return false;
    if (ss.peek() != '/') {
      ss >> index;
      --index;
      FinalVertices->push_back(3*index);
    }
  }
  
  return true;
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
bool ObjLoader<Texture, Normal>::parseMaterial() {
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

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
bool ObjLoader<Texture, Normal>::parseMatLib() { 
  ObjMaterialLoader* NewMatLoad = new ObjMaterialLoader(DirectoryPath);
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

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
void ObjLoader<Texture, Normal>::removeRedundantVertices(ParseIndexTy *indices) {

  ParseIndexTy VerticesNum = CurrentIndex;
  IndexVector &Vertices = *FinalVertices;
  ParseDataTy min = std::numeric_limits<ParseDataTy>::max(), max = std::numeric_limits<ParseDataTy>::min();
  //Compute classes for vertices , a class is the sum of the position components.
  for (unsigned int i = 0; i < VerticesNum; ++i) {
    ParseIndexTy Index = Vertices[i*NumAttribs];
    ParseDataTy VClass = PosData->at(Index) + PosData->at(Index+1) + PosData->at(Index+2);
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
    unsigned int IVClass = computeVertexClass(Vertices[i*NumAttribs], Offset, MultFact);
    ClassMap[IVClass].push_back(i);
  }
  std::cout << "Max Original Idx: " << *std::max_element(indices, indices + CurrentIndex) << std::endl;
  //Verify specifically each class
  for (unsigned int i = 0 ; i < BucketNum; ++i) {
    IndexVector &CurrClass = ClassMap[i];
    if (CurrClass.empty())
      continue;
    
    int norm_idx_off = (HasTex) ? 1 : 0;
    for (IndexVectorIt I = CurrClass.begin(), E = CurrClass.end(); I != E-1; ++I)
      for (IndexVectorIt I2 = I+1; I2 != E; ++I2) {
        ParseIndexTy i1 = (*I)*NumAttribs;
        ParseIndexTy i2 = (*I2)*NumAttribs;
        ParseIndexTy IDX1 = Vertices[i1];
        ParseIndexTy IDX2 = Vertices[i2];
        if (IDX1 == IDX2) {
          indices[*I2] = *I;
          continue;
        }
        bool equal = true;
        //Check Vertex Position Data
        if (!(fabsf(PosData->at(IDX1) - PosData->at(IDX2)) < threshold &&
            fabsf(PosData->at(IDX1+1) - PosData->at(IDX2+1)) < threshold &&
            fabsf(PosData->at(IDX1+2) - PosData->at(IDX2+2)) < threshold))
          equal = false;
        IDX1 = Vertices[i1+1];
        IDX2 = Vertices[i2+1];
        if (HasTex && equal && !(fabsf(TextureData->at(IDX1) - TextureData->at(IDX2)) < threshold &&
            fabsf(TextureData->at(IDX1+1) - TextureData->at(IDX2+1)) < threshold))
          equal = false;
        IDX1 = Vertices[i1+norm_idx_off+1];
        IDX2 = Vertices[i2+norm_idx_off+1];
        if (HasNorm && equal && !(fabsf(NormalData->at(IDX1) - NormalData->at(IDX2)) < threshold &&
            fabsf(NormalData->at(IDX1+1) - NormalData->at(IDX2+1) < threshold &&
            fabsf(NormalData->at(IDX1+2) - NormalData->at(IDX2+2)) < threshold)))
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
  IndexVector *NewFinalVertices = new IndexVector(Size*NumAttribs);
  NewFinalVertices->reserve(Size);
  //Clean up the vertices and correct the indices
  for (ParseIndexTy i = 1; i < CurrentIndex; ++i) {
    ParseIndexTy *ReturnedIndex = std::lower_bound(OrderedIndices, NewEnd, indices[i]);
    ParseIndexTy Pos = ReturnedIndex - OrderedIndices;
    indices[i] = Pos;
    //Vertex not removed, copy to final vertex
  }

  for (ParseIndexTy i = 0; i < Size; ++i) {
    IndexVectorIt StartIt = FinalVertices->begin() + *(OrderedIndices+i)*NumAttribs;
    std::copy(StartIt, StartIt+NumAttribs, NewFinalVertices->begin() + i*NumAttribs);
  }
  delete [] OrderedIndices;
  delete FinalVertices;
  FinalVertices = NewFinalVertices;

  std::cout << "Original Vertex num: " << VerticesNum << std::endl;
  std::cout << "Final Vertices size: " << FinalVertices->size()/NumAttribs << std::endl;
  std::cout << "Max index: " << FinalVertices->size()/NumAttribs << std::endl;
  std::cout << "True Max Index: " << *std::max_element(indices, indices + CurrentIndex) << std::endl; 
  std::cout << "Threshold: " << threshold << std::endl;
  std::cout << "Max: " << max << std::endl;
  std::cout << "Min: " << min << std::endl;
}

}

#endif
