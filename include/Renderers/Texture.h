#ifndef __UBERNGINE_TEXTURE_H__
#define __UBERNGINE_TEXTURE_H__

//struct TextureData;

namespace Uberngine {

class PureTexture {

//  TextureData* Texture;

protected:
//  PureTexture(TextureData* texture) : Texture(texture) {}
  PureTexture() {}
};

template<typename RendererType>
class Texture : public PureTexture {};

}

#endif
