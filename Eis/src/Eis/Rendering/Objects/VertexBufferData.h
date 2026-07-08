#pragma once

#include "ShaderData.h"

#include <vector>


namespace Eis
{
	class VertexBufferData
	{
	public:
		void SetLayout(const AttributeLayout& layout, uint64_t maxVertices);

		template<typename T>
		void Set(uint64_t vertexIndex, AttribSemantic semantic, const T& value)
		{
			EIS_CORE_ASSERT(vertexIndex < m_MaxVertexCount);

			const VertexAttribute& attrib = GetAttribBySemantic(semantic);

			EIS_CORE_ASSERT(sizeof(T) == attrib.ByteSize);

			memcpy(m_Data.data() + vertexIndex * m_Layout.Stride + attrib.ByteOffset, &value, attrib.ByteSize);
		}

		const void* GetData() const { return m_Data.data(); }
		uint64_t GetDataSize() const { return m_Data.size(); }
		uint32_t GetVertexDataStride() const { return m_Layout.Stride; }
		const AttributeLayout& GetLayout() const { return m_Layout; }

		bool Has(AttribSemantic semantic) const;

	private:
		const VertexAttribute& GetAttribBySemantic(AttribSemantic semantic) const;

	private:
		AttributeLayout m_Layout;

		std::vector<uint8_t> m_Data;
		uint64_t m_MaxVertexCount{};
	};
}