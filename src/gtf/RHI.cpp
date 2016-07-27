//
//  RHI.cpp
//  MeshPainter
//
//  Created by David Gallardo on 17/10/15.
//  Copyright (c) 2015 David Gallardo. All rights reserved.
//

#include "gtf/RHI.h"

namespace gtf 
{
RHI* GRHI(nullptr);

void RHITexture2D::setup(RHITextureInfo const & info)
{
    setup(info.internalFormat,
          info.bufferW,
          info.bufferH,
          info.format,
          info.type);
}

void RHITexture2D::setup(RHITexturePreset const & preset)
{
    setup(preset.internalFormat,
          preset.bufferW,
          preset.bufferH,
          preset.format,
          preset.type,
          preset.buffer,
          preset.texParams,
          preset.numParams);
}
} //namespace gtf