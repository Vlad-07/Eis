#include "Eispch.h"
#include "Mesh.h"

#include "VertexBufferData.h"
#include "Eis/Assets/AssetManager.h"

#include "Eis/Assets/Importers/ShaderImporter.h"


namespace Eis
{
	void Mesh::AddSubMesh(std::vector<MeshVertex>&& vertices, std::vector<uint32_t>&& indices, AssetHandle material)
	{
		m_SubMeshes.emplace_back((uint32_t)m_Indices.size(), (uint32_t)indices.size(), material);

		m_Vertices.reserve(m_Vertices.size() + vertices.size());
		m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());

		m_Indices.reserve(m_Indices.size() + indices.size());
		for (uint32_t i : indices)
			m_Indices.push_back(i + (uint32_t)m_Vertices.size());

		// TEMP

		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();

		VertexBufferData data;
		data.SetLayout(ShaderImporter::LoadShader("resources/shaders/Mesh.glsl")->GetAttributeLayout(), m_Vertices.size());
		for (size_t i{}; i < m_Vertices.size(); i++)
		{
			const MeshVertex& vertex = m_Vertices[i];

			if (data.Has(AttribSemantic::Position))
				data.Set(i, AttribSemantic::Position, vertex.Position);
			if (data.Has(AttribSemantic::Normal) && vertex.Normal.has_value())
				data.Set(i, AttribSemantic::Normal, *vertex.Normal);
			if (data.Has(AttribSemantic::TexCoord0) && vertex.TexCoord.has_value())
				data.Set(i, AttribSemantic::TexCoord0, *vertex.TexCoord);
		}

		Ref<VertexBuffer> vb = VertexBuffer::Create(data);

		Ref<IndexBuffer> ib = IndexBuffer::Create(m_Indices.data(), (uint32_t)m_Indices.size());

		m_VertexArray->AddVertexBuffer(vb);
		m_VertexArray->SetIndexBuffer(ib);
	}
}