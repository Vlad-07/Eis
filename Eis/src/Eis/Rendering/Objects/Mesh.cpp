#include "Eispch.h"
#include "Mesh.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLMesh.h"


namespace Eis
{
	Ref<StaticMesh> StaticMesh::Create(std::vector<MeshVertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<SubMesh>&& subMeshes)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			case RendererAPI::API::WebGL:
				return CreateRef<OpenGLStaticMesh>(std::move(vertices), std::move(indices), std::move(subMeshes));

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}
}