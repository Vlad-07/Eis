#pragma once

#include "Eis/Assets/Asset.h"

#include <optional>


#include "Eis/Rendering/Objects/VertexArray.h"


namespace Eis
{
	struct MeshVertex
	{
		glm::vec3 Position{};
		std::optional<glm::vec3> Normal{};
		std::optional<glm::vec4> Tangent{};
		std::optional<glm::vec2> TexCoord{};
		//glm::vec2 TexCoord1;
		//glm::vec4 Color;
		//glm::vec4 Joints0;
		//glm::vec4 Weights0;
	};

	struct SubMesh
	{
		uint32_t FirstIndex{};
		uint32_t IndexCount{};
		AssetHandle Material{ 0 };
	};

	class Mesh : public Asset
	{
	public:
		virtual ~Mesh() = default;

		void AddSubMesh(std::vector<MeshVertex>&& vertices, std::vector<uint32_t>&& indices, AssetHandle material);

		Ref<VertexArray> GetVA() { return m_VertexArray; }
		const std::vector<SubMesh>& GetSubMeshes() const { return m_SubMeshes; }

		static AssetType GetStaticType() { return AssetType::Mesh; }
		virtual AssetType GetType() const override { return GetStaticType(); }

	private:
		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		std::vector<SubMesh> m_SubMeshes;


		Ref<VertexArray> m_VertexArray;
	};
}