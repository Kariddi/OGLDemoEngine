#ifndef __OBJLOADER_LOADER_H__
#define __OBJLOADER_LOADER_H__
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <Mesh.h>
#include <Defs.h>
#include <MaterialLoader.h>
#include <LoaderTraits.h>

//using namespace std;
#define REGISTER_ERROR(message) if (record_errors && !no_error) \
                                  Errors.push_back(message + "\n");

namespace ObjLoader {

class MaterialLoader;

class Loader {
  enum Tokens { VERT, VERT_TEX, VERT_NORM, FACE, MTR_LIB, 
                MTR, END, UNKN};

  typedef std::map<std::string, MaterialLoader*> NameToMatLoaderMap;
  typedef NameToMatLoaderMap::iterator NameToMatLoaderMapIt;
  typedef float ParseDataTy;
  typedef uint32_t ParseIndexTy;
  typedef std::vector<ParseDataTy> DataVector;
  typedef std::vector<ParseIndexTy> IndexVector; 
  typedef DataVector::iterator DataVectorIt;
  typedef IndexVector::iterator IndexVectorIt;
//  typedef Uberngine::Mesh<Uberngine::Traits::MeshVertexType, Uberngine::Traits::MeshIDXType> MeshTy;
 
  struct LoaderPart {
    Uberngine::Material *Mat;
    ParseIndexTy MaxIndex;
  };

  static const unsigned char PadAmount[];
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
  std::vector<LoaderPart> *PartsVector;
  Uberngine::Mesh::TextureList TexList;
  LoaderPart CurrentPart;
  ParseIndexTy CurrentIndex;
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
  inline unsigned int computeVertexClass(ParseIndexTy index, unsigned int offset, float mult_fact) {
    return (unsigned int)(((*FinalVertices)[index] + (*FinalVertices)[index+1] + 
                                      (*FinalVertices)[index+2])*mult_fact + offset);
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

  template<typename T>
  T normalizeData(ParseDataTy Val); 
  
  template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
  void copyAndNormalizeVertexData(char *TargetVertices);

  template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
  Uberngine::Mesh *constructMesh();

  template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
  void copyAndNormalizeVertexData();

public:
  Loader(const std::string &dir_path) : DirectoryPath(dir_path) {}
  ~Loader() { clear(); }
  //Non-NULL on success
  template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
  Uberngine::Mesh *loadMesh(const std::string& filename, bool record_errors);
  Uberngine::Mesh *loadMesh(const std::string& filename, bool record_errors); 
  const std::vector<std::string>& getErrors() const { return Errors; }

  void clear() {  
    for (NameToMatLoaderMapIt I = MatLoadersMap.begin(), 
       E = MatLoadersMap.end(); I != E; ++I)
      delete I->second;
  }

  void clearErrors() { Errors.clear(); }
};

template<typename T>
T Loader::normalizeData(ParseDataTy Val) {
  const bool is_signed = std::numeric_limits<T>::is_signed;
  T RetVal;
  if (is_signed)
    RetVal = static_cast<T>(Val >= 0 ? Val * std::numeric_limits<T>::max() : Val * -std::numeric_limits<T>::min());
  else
    RetVal = static_cast<T>(Val * std::numeric_limits<T>::max());

  return RetVal;
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
void Loader::copyAndNormalizeVertexData(char *TargetVertices) {
  typedef typename Uberngine::LoaderElement<Uberngine::UBE_LOADER_FLOAT>::Type PositionTy;
  typedef typename Uberngine::LoaderElement<Texture>::Type TextureTy;
  typedef typename Uberngine::LoaderElement<Normal>::Type NormalTy;
  const uint8_t POff = Uberngine::LoaderElement<Uberngine::UBE_LOADER_FLOAT>::Size;
  const uint8_t TOff = Uberngine::LoaderElement<Texture>::Size;
  const uint8_t NOff = Uberngine::LoaderElement<Normal>::Size;
  const bool texture_is_signed = std::numeric_limits<TextureTy>::is_signed;
  const bool normal_is_signed = std::numeric_limits<NormalTy>::is_signed;

  unsigned int i = 0;
  for (DataVectorIt I = FinalVertices->begin(), E = FinalVertices->end(); I != E; ++I) {
    //Copy position data
    *(reinterpret_cast<PositionTy*>(&TargetVertices[i])) = *I++;
    i += POff;
    *(reinterpret_cast<PositionTy*>(&TargetVertices[i])) = *I++;
    i += POff;
     *(reinterpret_cast<PositionTy*>(&TargetVertices[i])) = *I++;
    i += POff;
    if (HasTex) { 
      if (Texture != Uberngine::UBE_LOADER_FLOAT) {
        *(reinterpret_cast<TextureTy*>(&TargetVertices[i])) = normalizeData<TextureTy>(*I++);
        i += TOff;
        *(reinterpret_cast<TextureTy*>(&TargetVertices[i])) = normalizeData<TextureTy>(*I++);
        i += TOff;
      } else {
        *(reinterpret_cast<TextureTy*>(&TargetVertices[i])) = *I++;
        i += TOff;
        *(reinterpret_cast<TextureTy*>(&TargetVertices[i])) = *I++;
        i += TOff;
      }
    }
    if (HasNorm) {
      if (Normal != Uberngine::UBE_LOADER_FLOAT) {
        *(reinterpret_cast<NormalTy*>(&TargetVertices[i])) = normalizeData<NormalTy>(*I++);
        i += NOff;
        *(reinterpret_cast<NormalTy*>(&TargetVertices[i])) = normalizeData<NormalTy>(*I++);
        i += NOff;
        *(reinterpret_cast<NormalTy*>(&TargetVertices[i])) = normalizeData<NormalTy>(*I);
        i += NOff; 
      } else {
        *(reinterpret_cast<NormalTy*>(&TargetVertices[i])) = *I++;
        i += NOff;
        *(reinterpret_cast<NormalTy*>(&TargetVertices[i])) = *I++;
        i += NOff;
        *(reinterpret_cast<NormalTy*>(&TargetVertices[i])) = *I;
        i += NOff; 
      }
    }
    i += PadAmount[i % 4];
  }
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
Uberngine::Mesh *Loader::constructMesh() {
  typedef typename Uberngine::LoaderElement<Texture>::Type TextureTy;
  typedef typename Uberngine::LoaderElement<Normal>::Type NormalTy; 
  const uint8_t TSize = Uberngine::LoaderElement<Texture>::Size;
  const uint8_t NSize = Uberngine::LoaderElement<Normal>::Size;
  const bool TSign = std::numeric_limits<TextureTy>::is_signed;
  const bool NSign = std::numeric_limits<NormalTy>::is_signed;


//  MeshTy *NewMesh;
  HasNorm = (NormalData->size() > 0);
  HasTex = (TextureData->size() > 0);
  delete VertexData;
  delete NormalData;
  delete TextureData;
  int ByteStride; 
  ByteStride = 3 * Uberngine::LoaderElement<Uberngine::UBE_LOADER_FLOAT>::Size;
  Stride = 3;
  if (HasNorm) {
    ByteStride += 3 * NSize;
    Stride += 3;
  }
  if (HasTex) {
    ByteStride += 2 * TSize;
    Stride += 2;
  }
  ByteStride += PadAmount[ByteStride % 4];
  
  ParseIndexTy *OptIndices = new ParseIndexTy[CurrentIndex];
  for (ParseIndexTy i = 0; i < CurrentIndex; ++i)
    OptIndices[i] = i;
  removeRedundantVertices(OptIndices);

  ParseIndexTy VertexNum = FinalVertices->size() / Stride;
  char *Vertices = new char[VertexNum * ByteStride];
  //Creating the final vertex vector
  //std::copy(FinalVertices->begin(), FinalVertices->end(), Vertices);
  copyAndNormalizeVertexData<Texture, Normal>(Vertices);
  delete FinalVertices;
  std::vector<Uberngine::Part*> PartVect;
  //Creating the indices vectors for each part. 
  ParseIndexTy PartIdx = 0;
  for (std::vector<LoaderPart>::iterator I = PartsVector->begin(), E = PartsVector->end(); I != E; ++I) {
    ParseIndexTy IdxSize = I->MaxIndex;
    ParseIndexTy IndicesSize = IdxSize - PartIdx + 1;
    ParseIndexTy MaxPartIndex = *std::max_element(OptIndices + PartIdx, OptIndices + IdxSize + 1);
    unsigned char IdxBytes = 4;
    if ((MaxPartIndex & 0xFF) == MaxPartIndex)
      IdxBytes = 1;
    else if ((MaxPartIndex & 0xFFFF) == MaxPartIndex)
      IdxBytes = 2;

    char *Indices = new char[IndicesSize*IdxBytes];
    Uberngine::Part *NewPart = NULL;
    if (IdxBytes == 1)
      NewPart = constructPart(reinterpret_cast<uint8_t*>(Indices), OptIndices, &(*I), PartIdx, IdxSize+1);
    else if (IdxBytes == 2)
      NewPart = constructPart(reinterpret_cast<uint16_t*>(Indices), OptIndices, &(*I), PartIdx, IdxSize+1);
    else if (IdxBytes == 4)
      NewPart = constructPart(reinterpret_cast<uint32_t*>(Indices), OptIndices, &(*I), PartIdx, IdxSize+1);
    PartIdx = IdxSize+1;
    PartVect.push_back(NewPart);
  }
  delete PartsVector;
  delete [] OptIndices;
  //Creating the new Mesh
  Uberngine::Mesh *NewMesh = new Uberngine::Mesh(Vertices, VertexNum, HasTex, HasNorm, 
                                                 Uberngine::Mesh::SizeSignedPair(TSize, TSign), 
                                                 Uberngine::Mesh::SizeSignedPair(NSize, NSign), PartVect, TexList);
  TexList.clear();

  return NewMesh;
}

template<Uberngine::ElementType Texture, Uberngine::ElementType Normal>
Uberngine::Mesh *Loader::loadMesh(const std::string& filename, bool record_errors) {
  VertexData = new DataVector();
  NormalData = new DataVector();
  TextureData = new DataVector();
  PartsVector = new std::vector<LoaderPart>();
  FinalVertices = new DataVector();
  CurrentIndex = 0;

  RecordErrors = record_errors;

  std::string full_pathname = DirectoryPath + filename;
  MeshFile.open(full_pathname.c_str(), std::ifstream::in);
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

  return constructMesh<Texture, Normal>();
}

}

#endif
