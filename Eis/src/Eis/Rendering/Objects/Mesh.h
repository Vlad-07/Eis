#pragma once

#include "Eis/Assets/Asset.h"

#include <optional>


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


	class VertexArray;

	class StaticMesh : public Asset
	{
	public:
		virtual ~StaticMesh() = default;

		virtual void Bind() const = 0;

		// TODO: remove after renderer rewrite
		virtual const Ref<VertexArray>& GetVA() const = 0;

		virtual const std::vector<SubMesh>& GetSubMeshes() const = 0;

		static Ref<StaticMesh> Create(std::vector<MeshVertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<SubMesh>&& subMeshes);

		static AssetType GetStaticType() { return AssetType::StaticMesh; }
		virtual AssetType GetType() const override { return GetStaticType(); }
	};
}