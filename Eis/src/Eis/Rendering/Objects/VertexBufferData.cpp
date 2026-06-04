#include "Eispch.h"
#include "VertexBufferData.h"


namespace Eis
{
	void VertexBufferData::SetLayout(const AttributeLayout& layout, uint64_t maxVertices)
	{
		m_Layout = layout;
		m_MaxVertexCount = maxVertices;

		m_Data.resize(m_MaxVertexCount * m_Layout.Stride, 0);
	}

	const VertexAttribute& VertexBufferData::GetAttribBySemantic(AttribSemantic semantic) const
	{
		for (const VertexAttribute& attrib : m_Layout.Attributes)
			if (attrib.Semantic == semantic)
				return attrib;

		EIS_CORE_ASSERT(false);
		return m_Layout.Attributes.front(); // hide the warning, this souldn't execute anyway
	}
}