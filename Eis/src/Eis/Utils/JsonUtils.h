#pragma once

#include "Eis/Assets/Asset.h"

#include <glm/glm.hpp>
#include <json/json.hpp>



namespace glm
{
	// Basic json

	static void to_json(nlohmann::json& j, Eis::AssetHandle handle)
	{
		j = (uint64_t)handle;
	}
	static void from_json(const nlohmann::json& j, Eis::AssetHandle& handle)
	{
		handle = j.get<uint64_t>();
	}

	static void to_json(nlohmann::json& j, vec2 v)
	{
		j = { v.x, v.y };
	}
	static void from_json(const nlohmann::json& j, vec2& v)
	{
		v.x = j[0]; v.y = j[1];
	}


	static void to_json(nlohmann::json& j, vec3 v)
	{
		j = { v.x, v.y, v.z };
	}
	static void from_json(const nlohmann::json& j, vec3& v)
	{
		v.x = j[0]; v.y = j[1]; v.z = j[2];
	}


	static void to_json(nlohmann::json& j, vec4 v)
	{
		j = { v.x, v.y, v.z, v.w };
	}
	static void from_json(const nlohmann::json& j, vec4& v)
	{
		v.x = j[0]; v.y = j[1]; v.z = j[2]; v.w = j[3];
	}



	// Ordered json

	static void to_json(nlohmann::ordered_json& j, Eis::AssetHandle handle)
	{
		j = (uint64_t)handle;
	}
	static void from_json(const nlohmann::ordered_json& j, Eis::AssetHandle& handle)
	{
		handle = j.get<uint64_t>();
	}

	static void to_json(nlohmann::ordered_json& j, vec2 v)
	{
		j = { v.x, v.y };
	}
	static void from_json(const nlohmann::ordered_json& j, vec2& v)
	{
		v.x = j[0]; v.y = j[1];
	}


	static void to_json(nlohmann::ordered_json& j, vec3 v)
	{
		j = { v.x, v.y, v.z };
	}
	static void from_json(const nlohmann::ordered_json& j, vec3& v)
	{
		v.x = j[0]; v.y = j[1]; v.z = j[2];
	}


	static void to_json(nlohmann::ordered_json& j, vec4 v)
	{
		j = { v.x, v.y, v.z, v.w };
	}
	static void from_json(const nlohmann::ordered_json& j, vec4& v)
	{
		v.x = j[0]; v.y = j[1]; v.z = j[2]; v.w = j[3];
	}
}