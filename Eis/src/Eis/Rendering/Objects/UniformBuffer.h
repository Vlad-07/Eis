#pragma once

#include "Eis/Core/Core.h"


namespace Eis
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, uint64_t size, uint64_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint64_t size, uint32_t binding);
	};
}