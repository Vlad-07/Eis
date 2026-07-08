#pragma once


namespace Eis
{
	enum class BaseDataType : uint8_t
	{
		None = 0,
		Bool,
		Int,
		Float
	};

	struct ShaderMemberType
	{
		std::string Name;

		BaseDataType DataType{};
		uint8_t VecSize{}; // 1 for non-vec, >1 for vec
		uint8_t Columns{}; // 1 for non-mat, >1 for mat

		uint8_t ByteSize{};
	};



	enum class AttribSemantic : uint8_t
	{
		None = 0,
		Position,     // a_Position
		Normal,       // a_Normal
		Tangent,      // a_Tangent
		Color,        // a_Color
		TexCoord0,    // a_TexCoord
		TexIndex,     // a_TexIndex
		TilingFactor, // a_TilingFactor
		EntityId      // a_EntityId
	};

	struct VertexAttribute : public ShaderMemberType
	{
		// location is the index in vector

		bool Normalized{}; // Atributes whose name ends with '_n' are normalized
		uint8_t ByteOffset{};
		AttribSemantic Semantic;
	};

	struct AttributeLayout
	{
		std::vector<VertexAttribute> Attributes;
		uint32_t Stride{};
	};



	struct UniformBufferMember : public ShaderMemberType
	{
		uint8_t ByteOffset{};
	};

	struct UniformBufferBlock
	{
		std::string Name;
		uint8_t Binding{};
		uint16_t BlockSize{};

		std::vector<UniformBufferMember> Members;
	};



	struct Sampler
	{
		std::string Name;
		uint8_t Binding;
	};



	struct FragmentOutput : public ShaderMemberType
	{
		// location is the index in vector
	};




	struct ShaderReflection
	{
		AttributeLayout VertexAttributes;
		std::vector<UniformBufferBlock> UniformBuffers;
		std::vector<Sampler> Samplers;
		std::vector<FragmentOutput> FragmentOutputs;
	};
}