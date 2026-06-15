#include "Eispch.h"
#include "ShaderReflector.h"

#include <spirv_cross.hpp>


namespace Eis
{
	static BaseDataType ShaderDataTypeFromSPIRType(spirv_cross::SPIRType::BaseType type)
	{
		switch (type)
		{
			case spirv_cross::SPIRType::Boolean: return BaseDataType::Bool;
			case spirv_cross::SPIRType::Int: return BaseDataType::Int;
			case spirv_cross::SPIRType::Float: return BaseDataType::Float;
			default: EIS_CORE_ASSERT(false); return BaseDataType::None;
		}
	}

	static uint32_t SPIRTypeSize(const spirv_cross::SPIRType& type)
	{
		uint32_t componentSize{};
		switch (type.basetype)
		{
			case spirv_cross::SPIRType::SByte:
			case spirv_cross::SPIRType::UByte:
			case spirv_cross::SPIRType::Boolean:
				componentSize = 1; break;
			case spirv_cross::SPIRType::Short:
			case spirv_cross::SPIRType::UShort:
				componentSize = 2; break;
			case spirv_cross::SPIRType::Int:
			case spirv_cross::SPIRType::UInt:
			case spirv_cross::SPIRType::Float:
				componentSize = 4; break;
			case spirv_cross::SPIRType::Int64:
			case spirv_cross::SPIRType::UInt64:
			case spirv_cross::SPIRType::Double:
				componentSize = 8; break;
			default: EIS_CORE_ASSERT(false); break;
		}
		return componentSize * type.vecsize * type.columns;
	}

	static AttribSemantic SemanticFromName(const std::string& name)
	{
		static const std::unordered_map<std::string, AttribSemantic> semanticMap
		{
			{ "a_Position", AttribSemantic::Position },
			{ "a_Normal", AttribSemantic::Normal },
			{ "a_Tangent", AttribSemantic::Tangent },
			{ "a_Color", AttribSemantic::Color },
			{ "a_TexCoord", AttribSemantic::TexCoord0 },
			{ "a_TexIndex", AttribSemantic::TexIndex },
			{ "a_TilingFactor", AttribSemantic::TilingFactor },
			{ "a_EntityId", AttribSemantic::EntityId }
		};

		auto it = semanticMap.find(name);
		if (it == semanticMap.end())
		{
			EIS_CORE_ASSERT(false);
			return AttribSemantic::None;
		}
		return it->second;
	}

    ShaderReflection Eis::ShaderReflector::Reflect(const ShaderBinaries& binaries)
	{
		ShaderReflection reflection;

		auto& vertexAtribs = reflection.VertexAttributes.Attributes;
		auto& samplers = reflection.Samplers;
		auto& uniformBuffers = reflection.UniformBuffers;
		auto& fragmentOutputs = reflection.FragmentOutputs;
		for (const auto& [stage, bin] : binaries)
		{
			spirv_cross::Compiler c{ bin };
			spirv_cross::ShaderResources resources = c.get_shader_resources();

			// Vertex attribs
			if (stage == ShaderStage::Vertex)
			{
				vertexAtribs.resize(resources.stage_inputs.size());
				for (size_t i{}; i < resources.stage_inputs.size(); i++)
				{
					const auto& input = resources.stage_inputs[i];
					const auto& spvType = c.get_type(input.type_id);

					const uint32_t location = c.get_decoration(input.id, spv::DecorationLocation);

					VertexAttribute& attrib = vertexAtribs[location];
					attrib.Name = input.name;
					attrib.DataType = ShaderDataTypeFromSPIRType(spvType.basetype);
					attrib.VecSize = spvType.vecsize;
					attrib.Columns = spvType.columns;
					attrib.ByteSize = SPIRTypeSize(spvType);

					attrib.Normalized = attrib.Name.ends_with("_n"); // maybe a better normalization method?
					attrib.Semantic = SemanticFromName(attrib.Name);
				}

				uint8_t offset{};
				for (auto& attrib : vertexAtribs)
				{
					attrib.ByteOffset = offset;
					offset += attrib.ByteSize;
				}
				reflection.VertexAttributes.Stride = offset;
			}

			// Samplers
			samplers.reserve(samplers.size() + resources.sampled_images.size());
			for (const auto& s : resources.sampled_images)
			{
				Sampler sampler;
				sampler.Name = s.name;
				sampler.Binding = c.get_decoration(s.id, spv::DecorationBinding);

				samplers.push_back(sampler);
			}

			// Uniform buffers
			uniformBuffers.reserve(resources.uniform_buffers.size());
			for (const auto& ub : resources.uniform_buffers)
			{
				const auto& ubType = c.get_type(ub.base_type_id);

				UniformBufferBlock uniformBuffer;
				uniformBuffer.Name = ub.name;
				uniformBuffer.Binding = (uint32_t)c.get_decoration(ub.id, spv::DecorationBinding);
				uniformBuffer.BlockSize = (uint32_t)c.get_declared_struct_size(ubType);

				uniformBuffer.Members.resize(ubType.member_types.size());
				for (uint32_t i{}; i < uniformBuffer.Members.size(); i++)
				{
					const auto& memberType = c.get_type(ubType.member_types[i]);

					UniformBufferMember& member = uniformBuffer.Members[i];
					member.Name = c.get_member_name(ub.base_type_id, i);
					member.DataType = ShaderDataTypeFromSPIRType(memberType.basetype);
					member.VecSize = memberType.vecsize;
					member.Columns = memberType.columns;
					member.ByteSize = (uint8_t)c.get_declared_struct_member_size(ubType, i);

					member.ByteOffset = c.type_struct_member_offset(ubType, i);
				}

				uniformBuffers.push_back(uniformBuffer);
			}

			// Shader output
			if (stage == ShaderStage::Fragment)
			{
				fragmentOutputs.resize(resources.stage_outputs.size());
				for (size_t i{}; i < resources.stage_outputs.size(); i++)
				{
					const auto& output = resources.stage_outputs[i];
					const auto& type = c.get_type(output.type_id);

					FragmentOutput& fragmentOut = fragmentOutputs[i];
					fragmentOut.Name = output.name;
					fragmentOut.DataType = ShaderDataTypeFromSPIRType(type.basetype);
					fragmentOut.VecSize = type.vecsize;
					fragmentOut.Columns = type.columns;
					fragmentOut.ByteSize = SPIRTypeSize(type);
				}
			}
		}

		return reflection;
	}
}