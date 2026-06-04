#pragma once

#include "ShaderData.h"


namespace Eis
{
	struct ShaderReflection
	{
		AttributeLayout VertexAttributes;
		std::vector<UniformBufferBlock> UniformBuffers;
		std::vector<Sampler> Samplers;
		std::vector<FragmentOutput> FragmentOutputs;
	};
}