#include "Eispch.h"
#include "MeshImporter.h"

#include "Eis/Project/Project.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/glm_element_traits.hpp>


namespace Eis
{
	Ref<Mesh> MeshImporter::ImportMesh(AssetHandle handle, const AssetMetadata& metadata)
	{
		return LoadMesh(Project::GetAssetsDir() / metadata.FilePath);
	}

	Ref<Mesh> MeshImporter::LoadMesh(const std::filesystem::path& path)
	{
		// not thread safe!
		static fastgltf::Parser gltfparser;

		auto data = fastgltf::GltfDataBuffer::FromPath(path);
		if (data.error() != fastgltf::Error::None)
		{
			EIS_CORE_ERROR("Failed to load gltf: {}", path.string());
			EIS_CORE_ERROR("{}: {}",
				fastgltf::getErrorName(data.error()),
				fastgltf::getErrorMessage(data.error()));
			return nullptr;
		}

		auto loadedAsset = gltfparser.loadGltf(data.get(), path.parent_path(),
			fastgltf::Options::LoadExternalBuffers);

		if (loadedAsset.error() != fastgltf::Error::None)
		{
			EIS_CORE_ERROR("Failed to parse gltf: {}", path.string());
			EIS_CORE_ERROR("{}: {}",
				fastgltf::getErrorName(loadedAsset.error()),
				fastgltf::getErrorMessage(loadedAsset.error()));
			return nullptr;
		}

		Ref<Mesh> mesh = CreateRef<Mesh>();
		fastgltf::Asset& asset = loadedAsset.get();

		fastgltf::iterateSceneNodes(asset, 0, fastgltf::math::fmat4x4{},
			[&](fastgltf::Node& node, fastgltf::math::fmat4x4 matrix)
			{
				if (!node.meshIndex.has_value())
					return;

				glm::mat4 transform = glm::make_mat4(matrix.data());

				fastgltf::Mesh& meshdata = asset.meshes[*node.meshIndex];
				for (auto& primitive : meshdata.primitives)
				{
					std::vector<MeshVertex> vertices;
					std::vector<uint32_t> indices;

					// Indices
					if (primitive.indicesAccessor.has_value())
					{
						auto& acc = asset.accessors[*primitive.indicesAccessor];
						indices.reserve(acc.count);

						fastgltf::iterateAccessor<uint32_t>(asset, acc,
							[&](uint32_t i)
							{
								indices.push_back(i);
							}
						);
					}

					// Position attrib
					// Prebake transform into vertices
					auto* posAttrib = primitive.findAttribute("POSITION");
					if (posAttrib != primitive.attributes.end())
					{
						auto& acc = asset.accessors[posAttrib->accessorIndex];
						vertices.resize(vertices.size() + acc.count);

						fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, acc,
							[&](glm::vec3 pos, size_t i)
							{
								vertices[i].Position = transform * glm::vec4{ pos, 1.0f };
							}
						);
					}

					// Normal attrib
					auto* normalAttrib = primitive.findAttribute("NORMAL");
					if (normalAttrib != primitive.attributes.end())
					{
						auto& acc = asset.accessors[normalAttrib->accessorIndex];

						fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, acc,
							[&](glm::vec3 normal, size_t i)
							{
								vertices[i].Normal = normal;
							}
						);
					}

					// TexCoord attrib
					auto* texcoord0Attrib = primitive.findAttribute("TEXCOORD_0");
					if (texcoord0Attrib != primitive.attributes.end())
					{
						auto& acc = asset.accessors[texcoord0Attrib->accessorIndex];

						fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, acc,
							[&](glm::vec2 uv, size_t i)
							{
								vertices[i].TexCoord = uv;
							}
						);
					}

					// Tangent attrib
					auto* tangentAttrib = primitive.findAttribute("TANGENT");
					if (tangentAttrib != primitive.attributes.end())
					{
						auto& acc = asset.accessors[tangentAttrib->accessorIndex];

						fastgltf::iterateAccessorWithIndex<glm::vec4>(asset, acc,
							[&](glm::vec4 tangent, size_t i)
							{
								vertices[i].Tangent = tangent;
							}
						);
					}

					// Material
					AssetHandle material{ 0 };
					if (primitive.materialIndex.has_value())
					{
						auto& mat = asset.materials[*primitive.materialIndex];

						// TODO: material system
					}

					mesh->AddSubMesh(std::move(vertices), std::move(indices), material);
				}
			}
		);

		return mesh;
	}
}