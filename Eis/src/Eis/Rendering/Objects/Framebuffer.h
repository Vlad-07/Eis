#pragma once

#include <vector>
#include <variant>

#include <glm/glm.hpp>


namespace Eis
{
	enum class FramebufferTexFormat
	{
		NONE = 0,

		R32I,

		RGB8,
		RGBA8,

		DEPTH16,
		DEPTH24,

		DEPTH24STENCIL8
	};

	struct DepthStencilClear
	{
		float Depth{ 1.0f };
		uint32_t Stencil{};

		DepthStencilClear() = default;
		DepthStencilClear(float depth, uint32_t stencil = 0)
			: Depth{ depth }, Stencil{ stencil } {}
	};

	using FramebufferClearValue = std::variant<int32_t, uint32_t, float, DepthStencilClear, glm::vec4>;

	struct FramebufferTexSpec
	{
		FramebufferTexSpec(FramebufferTexFormat format) : Format{ format } {}
		FramebufferTexSpec(FramebufferTexFormat format, FramebufferClearValue clearVal)
			: Format{ format }, ClearValue{ clearVal } {}

		FramebufferTexFormat Format;
		FramebufferClearValue ClearValue{ glm::vec4{ 0, 0, 0, 1 } }; // defaulting to vec4 might cause problems with int textures

		// TODO: wrap, filtering
	};

	struct FramebufferAttachmentsSpec
	{
		FramebufferAttachmentsSpec() = default;
		FramebufferAttachmentsSpec(std::initializer_list<FramebufferTexSpec> attachments)
			: Attachments{ attachments } {}

		std::vector<FramebufferTexSpec> Attachments;
	};

	struct FramebufferSpec
	{
		uint32_t Width{ 1 }, Height{ 1 };
		FramebufferAttachmentsSpec AttachmentsSpec;
		uint8_t Samples{ 1 };

		bool SwapChainTarget{ false };
	};


	enum class AttachmentBit : uint8_t
	{
		None = 0,
		Color,
		Depth,
		Stencil,
		DepthStencil
	};


	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Clear() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void BindTextures(uint32_t offset = 0) = 0;
		virtual void BindDepthTexture(uint32_t binding = 0) = 0;
		virtual uint32_t GetAttachmentsNumber() const = 0;

		// TODO: add filter params
		virtual void Blit(const Ref<Framebuffer>& target, AttachmentBit bit, uint32_t srcAttachment = 0, uint32_t dstAttachment = 0) const = 0;

		// Only for R32I!
		virtual int ReadPixel(uint32_t attachmentId, int x, int y) = 0;

		virtual const std::vector<uint32_t>& GetColorAttachmentsIds() const = 0;

		virtual const FramebufferSpec& GetSpec() const = 0;


		static Ref<Framebuffer> Create(const FramebufferSpec& fbSpec);
	};
}