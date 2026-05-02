#pragma once

#include <vector>
#include <variant>

#include <glm/glm.hpp>


namespace Eis
{
	enum class FramebufferTexFormat
	{
		NONE = 0,

		RGB8,
		RGBA8,

		DEPTH24STENCIL8
	};

	struct DepthStencilClear
	{
		float Depth{};
		uint32_t Stencil{};
	};

	using FramebufferClearValue = std::variant<uint32_t, float, DepthStencilClear, glm::vec4>;

	struct FramebufferTexSpec
	{
		FramebufferTexSpec() = default;
		FramebufferTexSpec(FramebufferTexFormat format) : Format{ format } {}
		FramebufferTexSpec(FramebufferTexFormat format, FramebufferClearValue clearVal)
			: Format{ format }, ClearValue{ clearVal } {}

		FramebufferTexFormat Format{ FramebufferTexFormat::NONE };
		FramebufferClearValue ClearValue{ glm::vec4{0.0f} }; // defaulting to vec4 might cause problems with int textures

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
		uint32_t Width{}, Height{};
		FramebufferAttachmentsSpec AttachmentsSpec;
		uint32_t Samples{ 1 };

		bool SwapChainTarget{ false };
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
		virtual uint32_t GetAttachmentsNumber() const = 0;

		virtual const std::vector<uint32_t>& GetColorAttachmentsIds() const = 0;


		static Ref<Framebuffer> Create(const FramebufferSpec& fbSpec);
	};
}