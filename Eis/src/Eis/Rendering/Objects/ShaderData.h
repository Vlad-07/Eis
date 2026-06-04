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

	struct ShaderMember
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
		Position,
		Normal,
		Tangent,
		Color,
		TexCoord0,
		TexIndex,
		TilingFactor,
		EntityId
	};

	struct VertexAttribute : public ShaderMember
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



	struct UniformBufferMember : public ShaderMember
	{
		uint8_t ByteOffset{};
	};

	struct UniformBufferBlock
	{
		std::string Name;
		uint8_t Binding{};
		uint32_t BlockSize{};

		std::vector<UniformBufferMember> Members;
	};



	struct Sampler
	{
		std::string Name;
		uint8_t Binding;
	};


	struct FragmentOutput : public ShaderMember
	{
		// location is the index in vector
	};
}