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

void RHITexture2D::setup(RHITextureInfo const & info, const void * data)
{
    setup(info.internalFormat,
          info.bufferW,
          info.bufferH,
          info.format,
          info.type,
		  data);
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

RHIVertexAttributeList::RHIVertexAttributeList(size_t structSize)
	: m_totalStructSize(structSize)
{
	m_totalStructSize = structSize;
	m_attrList.reserve(8);
}

void RHIVertexAttributeList::addAttribute(RHIVertexAttribute & attr)
{
	m_attrList.push_back(attr);
}

/*
Ej:
struct VertexStruct
{
	float posX, posY, posZ;
	float normX, normY, normZ;
	float tcX, tcY;
};

attrList.addAttribute(offsetof(VertexStruct, posX), 3, ERHIValueType::RHI_FLOAT, false);
attrList.addAttribute(offsetof(VertexStruct, normX), 3, ERHIValueType::RHI_FLOAT, false);
attrList.addAttribute(offsetof(VertexStruct, tcX), 2, ERHIValueType::RHI_FLOAT, false);
*/
void RHIVertexAttributeList::addAttribute(size_t offset, unsigned int components, ERHIValueType type, bool normalized)
{
	m_attrList.emplace_back(offset, components, type, normalized);
}

std::vector<RHIVertexAttribute> const & RHIVertexAttributeList::getIterableList() const
{
	return m_attrList;
}

unsigned int RHIVertexAttributeList::getStructSize() const
{
	return m_totalStructSize;
}

} //namespace gtf