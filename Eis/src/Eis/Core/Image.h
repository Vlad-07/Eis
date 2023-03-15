#pragma once

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

		Image(const std::string path, int flipVertically = 1);
		Image(uint8_t* data, int width, int height, int channels);
		Image(const Image& img);
		Image(Image&& img);
		~Image();

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetChannels() const { return m_Channels; }
		bool ValidImage() const { return m_IsValid; }
		uint8_t* GetData() const { return m_Data; }
		glm::vec3 GetPixel(int x, int y) const;

		void SetChannels(int channels) { m_Channels = channels; }
		Image Resize(int newWidth, int newHeight, int newChannels = 0);

		void SaveToDisk(const std::string name);

		uint8_t operator[](int i);

	private:
		uint8_t* m_Data;
		uint32_t m_Width, m_Height, m_Channels;
		bool m_IsValid;
	};
}