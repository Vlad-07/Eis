#pragma once

#include "Eis/Rendering/Objects/Mesh.h"

#include "Eis/Rendering/Objects/VertexArray.h"


namespace Eis
{
	class OpenGLStaticMesh : public StaticMesh
	{
	public:
		OpenGLStaticMesh(std::vector<MeshVertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<SubMesh>&& subMeshes);
		virtual ~OpenGLStaticMesh() = default;

		virtual void Bind() const override;

		virtual const Ref<VertexArray>& GetVA() const override { return m_VertexArray; }

		virtual const std::vector<SubMesh>& GetSubMeshes() const override { return m_SubMeshes; }

	private:
	//	std::vector<MeshVertex> m_Vertices;
	//	std::vector<uint32_t> m_Indices;
		std::vector<SubMesh> m_SubMeshes;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}