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
		std::optional<glm::vec2> TexCoord0{};
		std::optional<glm::vec2> TexCoord1{};
		//glm::vec4 Color;
		//glm::vec4 Joints0;
		//glm::vec4 Weights0;
	};


	class Mesh : public Asset
	{
	public:
		// ...


		static AssetType GetStaticType() { return AssetType::Mesh; }
		virtual AssetType GetType() const override { return GetStaticType(); }

	private:
		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	//	std::vector<AssetHandle> m_Materials;



		friend class MeshImporter;
	};
}