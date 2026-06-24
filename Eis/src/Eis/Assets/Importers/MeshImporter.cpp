#include "Eispch.h"
#include "MeshImporter.h"

#include "Eis/Project/Project.h"

#include "Eis/Assets/AssetManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/glm_element_traits.hpp>


namespace Eis
{
	namespace
	{
		template<typename T, typename F>
		void GetAttribute(std::string_view name, const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, F&& func)
		{
			auto* texcoord0Attrib = primitive.findAttribute(name);
			if (texcoord0Attrib != primitive.attributes.end())
			{
				const auto& acc = asset.accessors[texcoord0Attrib->accessorIndex];
				fastgltf::iterateAccessorWithIndex<T>(asset, acc, func);
			}
		}
	}



	Ref<Mesh> MeshImporter::ImportMesh(AssetHandle handle, const AssetMetadata& metadata)
	{
		return LoadMesh(Project::GetAssetDir() / metadata.FilePath);
	}

	Ref<Mesh> MeshImporter::LoadMesh(const std::filesystem::path& path)
	{
		// not thread safe!
		static fastgltf::Parser gltfparser;

		std::filesystem::path parentPath{ path.parent_path() };

		auto data = fastgltf::GltfDataBuffer::FromPath(path);
		if (data.error() != fastgltf::Error::None)
		{
			EIS_CORE_ERROR("Failed to load gltf: {}", path.string());
			EIS_CORE_ERROR("{}: {}",
				fastgltf::getErrorName(data.error()),
				fastgltf::getErrorMessage(data.error()));
			return nullptr;
		}

		auto loadedAsset = gltfparser.loadGltf(data.get(), parentPath,
			fastgltf::Options::LoadExternalBuffers);

		if (loadedAsset.error() != fastgltf::Error::None)
		{
			EIS_CORE_ERROR("Failed to parse gltf: {}", path.string());
			EIS_CORE_ERROR("{}: {}",
				fastgltf::getErrorName(loadedAsset.error()),
				fastgltf::getErrorMessage(loadedAsset.error()));
			return nullptr;
		}

		fastgltf::Asset& asset = loadedAsset.get();
		Ref<Mesh> mesh = CreateRef<Mesh>();

		/*std::vector<MeshVertex> vertices{8};
		vertices[0].Position = { -0.5, -0.5,  0.5, };
		vertices[1].Position = {  0.5, -0.5,  0.5, };
		vertices[2].Position = {  0.5,  0.5,  0.5, };
		vertices[3].Position = { -0.5,  0.5,  0.5, };
		vertices[4].Position = { -0.5, -0.5, -0.5, };
		vertices[5].Position = {  0.5, -0.5, -0.5, };
		vertices[6].Position = {  0.5,  0.5, -0.5, };
		vertices[7].Position = { -0.5,  0.5, -0.5, };


		std::vector<uint32_t> indices{
			0, 1, 2,    2, 3, 0,
			5, 4, 7,    7, 6, 5,
			4, 0, 3,    3, 7, 4,
			1, 5, 6,    6, 2, 1,
			4, 5, 1,    1, 0, 4,
			3, 2, 6,    6, 7, 3 };

		mesh->AddSubMesh(std::move(vertices), std::move(indices), 0);

		return mesh;//*/

		fastgltf::iterateSceneNodes(asset, 0, fastgltf::math::fmat4x4{},
			[&](fastgltf::Node& node, const fastgltf::math::fmat4x4& matrix) -> void
			{
				if (!node.meshIndex.has_value())
					return;

				const glm::mat4 transform = glm::make_mat4(matrix.data());

				const fastgltf::Mesh& meshdata = asset.meshes[*node.meshIndex];
				for (const fastgltf::Primitive& primitive : meshdata.primitives)
				{
					std::vector<MeshVertex> vertices;
					std::vector<uint32_t> indices;

					// Indices
					if (primitive.indicesAccessor.has_value())
					{
						const auto& acc = asset.accessors[*primitive.indicesAccessor];
						indices.reserve(acc.count);

						fastgltf::iterateAccessor<uint32_t>(asset, acc,
							[&](uint32_t i)
							{
								indices.push_back(i);
							});
					}

					// Attributes

					auto* posAttrib = primitive.findAttribute("POSITION");
					if (posAttrib != primitive.attributes.end())
					{
						const auto& acc = asset.accessors[posAttrib->accessorIndex];
						vertices.resize(vertices.size() + acc.count);

						fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, acc,
							[&](glm::vec3 pos, size_t i)
							{
								// Bake transform into vertices
								vertices[i].Position = transform * glm::vec4{ pos, 1.0f };
							});
					}

					GetAttribute<glm::vec3>("NORMAL", asset, primitive,
						[&](glm::vec3 normal, size_t i)
						{
							vertices[i].Normal = normal;
						});

					GetAttribute<glm::vec2>("TEXCOORD_0", asset, primitive,
						[&](glm::vec2 uv, size_t i)
						{
							uv.y = 1.0f - uv.y; // gltf 2.0 spec has origin in the top left corner (see 3.8.3)
							vertices[i].TexCoord = uv;
						});

					GetAttribute<glm::vec4>("TANGENT", asset, primitive,
						[&](glm::vec4 tangent, size_t i)
						{
							vertices[i].Tangent = tangent;
						});

					// Material
					AssetHandle material{ 0 };
					if (primitive.materialIndex.has_value())
					{
						auto& mat = asset.materials[*primitive.materialIndex];

						// TODO: material system

						const fastgltf::Texture& texture = asset.textures[mat.pbrData.baseColorTexture->textureIndex];

						const fastgltf::Image& image = asset.images[*texture.imageIndex];

						std::visit(fastgltf::visitor{
							[&](const fastgltf::sources::URI& uri)
							{
								material = Project::GetEditorAssetManager()->ImportAsset(parentPath / uri.uri.c_str());
							},
							[](auto&& c) {}
						}, image.data);
					}

					mesh->AddSubMesh(std::move(vertices), std::move(indices), material);
				}
			});

		mesh->Upload();

		return mesh;
	}
}