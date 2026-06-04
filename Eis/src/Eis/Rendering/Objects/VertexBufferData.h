#pragma once

#include "ShaderData.h"

#include <vector>


namespace Eis
{
	class VertexBufferData
	{
	public:
		VertexBufferData() = default;

		void SetLayout(const AttributeLayout& layout, uint64_t maxVertices);

		template<typename T>
		void Set(uint64_t vertexIndex, AttribSemantic semantic, const T& value)
		{
			EIS_CORE_ASSERT(vertexIndex < m_MaxVertexCount);

			const VertexAttribute& attrib = GetAttribBySemantic(semantic);

			EIS_CORE_ASSERT(sizeof(T) == attrib.ByteSize);

			memcpy(m_Data.data() + vertexIndex * m_Layout.Stride + attrib.ByteOffset, &value, attrib.ByteSize);
		}

		const void* GetData() { return m_Data.data(); }
		uint32_t GetVertexDataSize() const { return m_Layout.Stride; }

		void Clear() { m_Data.clear(); m_Layout = {}; m_MaxVertexCount = 0; }

	private:
		const VertexAttribute& GetAttribBySemantic(AttribSemantic semantic) const;

	private:
		AttributeLayout m_Layout;

		std::vector<uint8_t> m_Data;
		uint64_t m_MaxVertexCount{};
	};
}