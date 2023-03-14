#pragma once

#include <stb_image.h>
#include <glm/glm.hpp>

namespace Eis
{
	class Image
	{
	public:
		enum ColorFormat
		{
			None = 0, Y, R, G, B, RG, GB, RB, RGB
		};

		Image(const std::string path);
		Image(stbi_uc* data, int width, int height, int channels);
		~Image();

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetChannels() const { return m_Channels; }
		bool ValidImage() const { return m_IsValid; }
		stbi_uc* GetData() const { return m_Data; }
		glm::vec3 GetPixel(int x, int y);

		void SetChannels(int channels) { m_Channels = channels; }
		void ChangeColorFormat(ColorFormat format);
		Image Resize(int newWidth, int newHeight, int newChannels = 0);

		void SaveToDisk(const std::string name);

		uint8_t operator[](int i);

	private:
		stbi_uc* m_Data;
		int m_Width, m_Height, m_Channels;
		bool m_IsValid;
	};
}