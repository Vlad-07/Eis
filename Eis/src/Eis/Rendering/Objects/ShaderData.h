#pragma once


namespace Eis
{
	enum class ShaderDataType
	{
		None = 0,
		Bool,
		Int,
		Float
	};

	struct VertexAttribute
	{
		std::string Name;
		ShaderDataType DataType{};
		uint32_t ComponentCount{}; // 1 for non-vec, >1 for vec
		uint32_t Colums{}; // 1 for non-mat, >1 for mat
		uint32_t Size{}; // Size in bytes
		bool Normalized{}; // Atributes whose name ends with '_n' are normalized

		size_t Offset{}; // Calculated by layout, do not set
	};


	class AttributeLayout
	{
	public:
		AttributeLayout() = default;
		AttributeLayout(const std::initializer_list<VertexAttribute>& atribs) : m_Attributes(atribs)
		{
			CalculateOffAndStride();
		}
		AttributeLayout(const std::vector<VertexAttribute>& atribs) : m_Attributes(atribs)
		{
			CalculateOffAndStride();
		}

		const std::vector<VertexAttribute>& GetAttributes() const { return m_Attributes; }

		uint32_t GetStride() const { return m_Stride; }

		std::vector<VertexAttribute>::iterator begin() { return m_Attributes.begin(); }
		std::vector<VertexAttribute>::iterator end() { return m_Attributes.end(); }
		std::vector<VertexAttribute>::const_iterator begin() const { return m_Attributes.begin(); }
		std::vector<VertexAttribute>::const_iterator end() const { return m_Attributes.end(); }

	private:
		void CalculateOffAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Attributes)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<VertexAttribute> m_Attributes;
		uint32_t m_Stride = 0;
	};
}