#pragma once

//#include <gtf/RHI.h>

namespace gtf
{
	class Texture2DLoader
	{
	public:
		enum class ELoadingAction : unsigned char
		{
			IDLE,
			LOADING_FROM_FILE
		};

		static bool isTextureFile(const char * path);
		bool loadFromFile(const char * path, class Texture2D & texture);
		void getLoadingStatus(ELoadingAction & currentAction, float & pct) const;
	private:
		float m_loadedPct;
		ELoadingAction m_currentAction{ ELoadingAction::IDLE };
	};

	class Texture2D
	{
	public:
		~Texture2D();
		void const * getData() { return m_data; };
		unsigned int getChannels() const { return m_channels; };
		unsigned int getChannelBytes() const { return m_channelBytes; };
		unsigned int getWidth() const { return m_width; };
		unsigned int getHeight() const { return m_height; };
	private:
		void* m_data{ nullptr };
		unsigned int m_channels{ 0 };
		unsigned int m_channelBytes{ 0 };
		unsigned int m_width{ 0 };
		unsigned int m_height{ 0 };

		friend class Texture2DLoader;
	};

	struct RHITextureInfo;
	void fillRHITextureInfo(gtf::Texture2D & texture, gtf::RHITextureInfo & info);
}