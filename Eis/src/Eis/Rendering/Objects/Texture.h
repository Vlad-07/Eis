#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Core/Buffer.h"
#include "Eis/Assets/Asset.h"


namespace Eis
{
	enum class ImageFormat
	{
		None = 0,

		RGB8,
		RGBA8
	};

	struct TextureSpec
	{
		uint32_t Width = 1, Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
	//	bool GenerateMipmaps = true;
	};


	class Texture : public Asset
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpec& GetSpec() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererId() const = 0;

		virtual void SetData(Buffer data) = 0;
		
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};


	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpec& spec, Buffer data = Buffer{});

		static AssetType GetStaticType() { return AssetType::Texture2D; }
		virtual AssetType GetType() const override { return GetStaticType(); }
	};
}