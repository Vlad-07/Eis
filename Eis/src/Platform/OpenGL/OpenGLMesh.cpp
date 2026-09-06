#include "Eispch.h"
#include "OpenGLMesh.h"

#include "Eis/Rendering/Objects/VertexBufferData.h"

// temp
#include "Eis/Assets/Importers/ShaderImporter.h"


namespace Eis
{
	OpenGLStaticMesh::OpenGLStaticMesh(std::vector<MeshVertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<SubMesh>&& subMeshes)
	{
		EIS_CORE_ASSERT(vertices.size() && indices.size() && subMeshes.size());

		//m_Vertices = std::move(vertices);
		//m_Indices = std::move(indices);
		m_SubMeshes = std::move(subMeshes);


		// Calculate bounds

		m_BoundingBox.Min = glm::vec3{ std::numeric_limits<float>::max() };
		m_BoundingBox.Max = glm::vec3{ std::numeric_limits<float>::lowest() };
		for (MeshVertex& v : vertices)
		{
			m_BoundingBox.Min = glm::min(m_BoundingBox.Min, v.Position);
			m_BoundingBox.Max = glm::max(m_BoundingBox.Max, v.Position);
		}
		// maybe a thigter bounding sphere?
		m_BoundingSphere.Center = (m_BoundingBox.Min + m_BoundingBox.Max) / 2.0f;
		m_BoundingSphere.Radius = glm::length(m_BoundingBox.Max - m_BoundingSphere.Center);


		// Upload to GPU

		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();

		VertexBufferData data;

		// TODO:
		data.SetLayout(ShaderImporter::LoadShader("resources/shaders/Mesh.glsl")->GetAttributeLayout(), vertices.size());

		for (size_t i{}; i < vertices.size(); i++)
		{
			const MeshVertex& vertex = vertices[i];

			if (data.Has(AttribSemantic::Position))
				data.Set(i, AttribSemantic::Position, vertex.Position);
			if (data.Has(AttribSemantic::Normal) && vertex.Normal.has_value())
				data.Set(i, AttribSemantic::Normal, *vertex.Normal);
			if (data.Has(AttribSemantic::TexCoord0) && vertex.TexCoord.has_value())
				data.Set(i, AttribSemantic::TexCoord0, *vertex.TexCoord);
		}

		m_VertexBuffer = VertexBuffer::Create(data);

		m_IndexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	void OpenGLStaticMesh::Bind() const
	{
		m_VertexArray->Bind();
	}
}