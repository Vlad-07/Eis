#include <Eispch.h>
#include "Image.h"

#include <Eis/Core/Log.h>

#include <stb_image.h>
#include <stb_image_resize.h>
#include <stb_image_write.h>

Eis::Image::Image(const std::string path, int flipVertically) : m_IsValid(false)
{
	int width = 0, height = 0, channels = 0;
	stbi_set_flip_vertically_on_load(flipVertically);
	uint8_t* data = (uint8_t*)stbi_load(path.c_str(), &width, &height, &channels, channels);
	if (!data)
	{
		EIS_ERROR("Invalid image path: {0}\n\n", path);
		m_Data = 0;
		m_Width = 0;
		m_Height = 0;
		m_Channels = 0;
		return;
	}

	m_IsValid = true;
	m_Data = data;
	m_Width = width;
	m_Height = height;
	m_Channels = channels;
}

Eis::Image::Image(uint8_t* data, int width, int height, int channels) : m_IsValid(true)
{
	m_Data = data;
	m_Width = width;
	m_Height = height;
	m_Channels = channels;
}

Eis::Image::Image(const Image& img) : m_IsValid(true)
{
	m_Width = img.GetWidth();
	m_Height = img.GetHeight();
	m_Channels = img.GetChannels();

	delete[] m_Data;
	m_Data = new uint8_t[m_Width * m_Height * m_Channels];

	for (uint32_t i = 0; i < m_Width * m_Height * m_Channels; i++)
		m_Data[i] = img.GetData()[i];
}

Eis::Image::Image(Image&& img) : m_IsValid(true)
{
	m_Width = img.GetWidth();
	m_Height = img.GetHeight();
	m_Channels = img.GetChannels();
	m_Data = img.GetData();

	img.m_Data = nullptr;
}

Eis::Image::~Image()
{
	stbi_image_free(m_Data);
}

glm::vec3 Eis::Image::GetPixel(uint32_t x, uint32_t y) const
{
	if (x < 0 || y < 0 || x > m_Width || y > m_Height)
		EIS_ERROR("Invalid pixel requested!");

	return { m_Data[x * m_Channels + y * m_Width * m_Channels],
			 m_Data[x * m_Channels + y * m_Width * m_Channels + 1],
			 m_Data[x * m_Channels + y * m_Width * m_Channels + 2] };
}

Eis::Image Eis::Image::Resize(int newWidth, int newHeight, int newChannels)
{
	if (newChannels == 0)
		newChannels = m_Channels;

	uint8_t* newData = new uint8_t[newWidth * newHeight * m_Channels];
	stbir_resize_uint8(m_Data, m_Width, m_Height, 0, newData, newWidth, newHeight, 0, newChannels);

	return Image(newData, newWidth, newHeight, newChannels);
}

void Eis::Image::SaveToDisk(const std::string name)
{
	auto i = name.find_last_of(".");
	std::string type = name.substr(i);

	if (type == ".png")
		stbi_write_png(name.c_str(), m_Width, m_Height, m_Channels, m_Data, 0);
	else if (type == ".jpg")
		stbi_write_jpg(name.c_str(), m_Width, m_Height, m_Channels, m_Data, 0);
	else if (type == ".bmp")
		stbi_write_bmp(name.c_str(), m_Width, m_Height, m_Channels, m_Data);
	else if (type == ".tga")
		stbi_write_tga(name.c_str(), m_Width, m_Height, m_Channels, m_Data);
	else
		EIS_ERROR("Invalid file type given for: '{0}'!\n", name)
}

uint8_t Eis::Image::operator[](int i)
{
	return m_Data[i];
}