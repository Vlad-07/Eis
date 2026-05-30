#pragma once

#include "Eis/Rendering/Objects/UniformBuffer.h"


typedef unsigned int GLuint;

namespace Eis
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint64_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint64_t size, uint64_t offset = 0) override;

	private:
		GLuint m_RendererId{};
		uint64_t m_Size{};
	};
}