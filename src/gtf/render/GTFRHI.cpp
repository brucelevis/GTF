//
//  GTFRHI.cpp
//  MeshPainter
//
//  Created by David Gallardo on 17/10/15.
//  Copyright (c) 2015 David Gallardo. All rights reserved.
//

#include "GTFRHI.h"

GTFRHI* RHI(nullptr);

void GTFRHITexture2D::setup(GTFRHITextureInfo const & info)
{
    setup(info.internalFormat,
          info.bufferW,
          info.bufferH,
          info.format,
          info.type);
}

void GTFRHITexture2D::setup(GTFRHITexturePreset const & preset)
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
