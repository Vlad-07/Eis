#pragma once

#include <vector>

#include "Eis/Core/Layer.h"
#include "Eis/Debug/Assert.h"


namespace Eis
{
	class LayerLib
	{
	public:
		using LayerFactory = std::function<Scope<Layer>(std::optional<Buffer>&)>;

		// Default factory for default constructible or Buffer recieving layers
		template<typename T>
		static Scope<Layer> DefaultFactory(std::optional<Buffer>& data)
		{
			static_assert(std::is_base_of<Layer, T>(), "Registered layers must derive Eis::Layer!");
			if constexpr (std::is_constructible<T, Buffer&>::value)
			{
				if (data)
					return Eis::CreateScope<T>(data.value());
				// else falltrough to default construction
			}

			if constexpr (std::is_default_constructible<T>::value)
				return Eis::CreateScope<T>();
			else
			{
				EIS_ASSERT(false, "T is not default constructible and nullopt was provided or T is not suitable for default factory!");
				return nullptr;
			}
		}

	public:
		LayerLib() = default;
		~LayerLib() = default;

		void SetContext(const EisContext& context) { m_Context = context; }

		void RegisterLayer(const LayerFactory& factory, const std::string& name);

		Scope<Layer> MakeLayer(const std::string& name, std::optional<Buffer>& data) const;
		Scope<Layer> MakeLayer(int32_t id, std::optional<Buffer>& data) const;

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