#pragma once

#include "ShaderData.h"


namespace Eis
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const AttributeLayout& GetLayout() const = 0;
		virtual void SetLayout(const AttributeLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* verticies, uint32_t size);
	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};


	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, uint64_t size, uint64_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint64_t size, uint32_t binding);
	};
}