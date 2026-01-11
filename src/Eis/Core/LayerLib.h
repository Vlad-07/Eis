#pragma once

#include <vector>

#include "Eis/Core/Layer.h"


namespace Eis
{
	class LayerLib
	{
	public:
		using LayerFactory = std::function<Scope<Layer>(std::optional<Buffer>)>;

		template<typename T>
		static Scope<Layer> DefaultFactory(const std::optional<Buffer>& data = std::nullopt)
		{
			if constexpr (std::is_constructible<T, Buffer>())
			{
				return Eis::CreateScope<T>(data);
			}
			else
			{
				static_assert(std::is_constructible<T>(), "Layers must be default constructible or take Eis::Buffer!");
				return Eis::CreateScope<T>();
			}
		}

	public:
		LayerLib() = default;
		~LayerLib() = default;

		void SetContext(const EisContext& context) { m_Context = context; }

		void RegisterLayer(const LayerFactory& factory, const std::string& name);

		Scope<Layer> MakeLayer(const std::string& name, std::optional<Buffer> data = std::nullopt) const;
		Scope<Layer> MakeLayer(int32_t id, std::optional<Buffer> data = std::nullopt) const;

		int32_t GetLayerId(const std::string& name) const;

	private:
		struct NamedFactory
		{
			std::string Name;
			LayerFactory Factory;
		};

	private:
		std::vector<NamedFactory> m_LayerFactories;
		EisContext m_Context;
	};
}