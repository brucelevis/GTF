#include "gtf/Texture2D.h"
#include "gtf/LangUtils.h"
#include "gtf/RHI.h"

#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace gtf
{
	bool Texture2DLoader::isTextureFile(const char * path)
	{
		char ext[6] = { 0 };
		getFileExtension(path, ext, sizeof(ext));
		static const char* meshExtensions[] = { ".jpg" , ".jpeg", ".png", ".tga", ".hdr" };
		bool result = false;

		for (int i = 0; i < 5; ++i)
		{
			result |= (strcmp(ext, meshExtensions[i]) == 0);
		}

		return result;
	}

	bool Texture2DLoader::loadFromFile(const char * path, class Texture2D & texture)
	{
		char ext[6] = { 0 };
		getFileExtension(path, ext, sizeof(ext));

		m_currentAction = ELoadingAction::LOADING_FROM_FILE;
		m_loadedPct = 0.0f;

		if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
		{
			if (!true) return false;
		}
		else if (strcmp(ext, ".png") == 0)
		{
			if (!true) return false;
		}
		else if (strcmp(ext, ".tga") == 0)
		{
			if (!true) return false;
		}
		else
		{
			return false;
		}

		int width, height, components;
		int isHDR = stbi_is_hdr(path);

		if (!isHDR)
		{
			stbi_set_flip_vertically_on_load(true);
			texture.m_data = stbi_load(path, &width, &height, &components, 0);
		}
		else
		{
			texture.m_data = stbi_loadf(path, &width, &height, &components, 0);
		}

		if (texture.m_data != nullptr)
		{
			texture.m_width = width;
			texture.m_height = height;
			texture.m_channelBytes = (isHDR) ? 4 : 1;
			texture.m_channels = components;

			return (texture.m_channels == 3) || (texture.m_channels == 4);
		}

		return false;
	}

	void Texture2DLoader::getLoadingStatus(ELoadingAction & currentAction, float & pct) const
	{
		currentAction = m_currentAction;
		pct = m_loadedPct;
	}

	Texture2D::~Texture2D()
	{
		if(m_data != nullptr)
			stbi_image_free(m_data);
	}


	void fillRHITextureInfo(Texture2D & texture, RHITextureInfo & info)
	{
		info.bufferH = texture.getHeight();
		info.bufferW = texture.getWidth();

		if (texture.getChannelBytes() == 4) //HDRI
		{
			info.format = gtf::ERHITexFormat::RHI_RGB;
			info.internalFormat = gtf::ERHITexInternalFormat::RHI_RGB32F;
			info.type = gtf::ERHIValueType::RHI_FLOAT;
		}
		else if (texture.getChannels() == 3) //RGB
		{
			info.format = gtf::ERHITexFormat::RHI_RGB;
			info.internalFormat = gtf::ERHITexInternalFormat::RHI_RGB8;
			info.type = gtf::ERHIValueType::RHI_UNSIGNED_BYTE;
		}
		else if (texture.getChannels() == 4) //RGBA
		{
			info.format = gtf::ERHITexFormat::RHI_RGBA;
			info.internalFormat = gtf::ERHITexInternalFormat::RHI_RGBA8;
			info.type = gtf::ERHIValueType::RHI_UNSIGNED_BYTE;
		}
	}
}