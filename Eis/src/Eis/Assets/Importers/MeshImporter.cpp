#include "Eispch.h"
#include "MeshImporter.h"

#include "Eis/Project/Project.h"

#include <glm/glm.hpp>

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
			return nullptr;
		}

		auto loadedAsset = gltfparser.loadGltf(data.get(), path.parent_path(),
			fastgltf::Options::LoadExternalBuffers);

		if (loadedAsset.error() != fastgltf::Error::None)
		{
			EIS_CORE_ERROR("Failed to parse gltf: {}", path.string());
			return nullptr;
		}

		Ref<Mesh> mesh = CreateRef<Mesh>();
		fastgltf::Asset& asset = loadedAsset.get();
		for (auto& meshdata : asset.meshes)
		{
			for (auto& primitive : meshdata.primitives)
			{
				uint32_t baseVertex = (uint32_t)mesh->m_Vertices.size();
				
				// Indices
				if (primitive.indicesAccessor.has_value())
				{
					auto& acc = asset.accessors[*primitive.indicesAccessor];
					mesh->m_Indices.reserve(mesh->m_Indices.size() + acc.count);

					fastgltf::iterateAccessor<uint32_t>(asset, acc,
						[&](uint32_t i)
						{
							mesh->m_Indices.push_back(baseVertex + i);
						}
					);
				}

				// Position attrib
				auto* posAttrib = primitive.findAttribute("POSITION");
				if (posAttrib != primitive.attributes.end())
				{
					auto& acc = asset.accessors[posAttrib->accessorIndex];
					mesh->m_Vertices.resize(mesh->m_Vertices.size() + acc.count);

					fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, acc,
						[&](glm::vec3 pos, size_t i)
						{
							mesh->m_Vertices[baseVertex + i].Position = pos;
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
							mesh->m_Vertices[baseVertex + i].Normal = normal;
						}
					);
				}

				// TexCoord0 attrib
				auto* texcoord0Attrib = primitive.findAttribute("TEXCOORD_0");
				if (texcoord0Attrib != primitive.attributes.end())
				{
					auto& acc = asset.accessors[texcoord0Attrib->accessorIndex];

					fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, acc,
						[&](glm::vec2 uv, size_t i)
						{
							mesh->m_Vertices[baseVertex + i].TexCoord0 = uv;
						}
					);
				}

				// TexCoord1 attrib
				auto* texcoord1Attrib = primitive.findAttribute("TEXCOORD_1");
				if (texcoord1Attrib != primitive.attributes.end())
				{
					auto& acc = asset.accessors[texcoord1Attrib->accessorIndex];

					fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, acc,
						[&](glm::vec2 uv, size_t i)
						{
							mesh->m_Vertices[baseVertex + i].TexCoord1 = uv;
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
							mesh->m_Vertices[baseVertex + i].Tangent = tangent;
						}
					);
				}

				// Material
				if (primitive.materialIndex.has_value())
				{
					auto& mat = asset.materials[*primitive.materialIndex];
					
					// TODO: material system
				}
			}
		}

		return mesh;
	}
}