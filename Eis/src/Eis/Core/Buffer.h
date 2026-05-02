#pragma once

#include <cstdint>


namespace Eis
{
	class Buffer
	{
	public:
		Buffer() noexcept = default;
		Buffer(const void* data, uint64_t size) noexcept;
		Buffer(uint64_t size) noexcept;
		Buffer(const Buffer& buf) noexcept;
		Buffer(Buffer&& buf) noexcept;
		~Buffer() noexcept;

		const Buffer& operator=(const Buffer& other);

		void Allocate(uint64_t size);
		void Resize(uint64_t size);
		void Release();

		void ZeroInit();
		void AppendNull();

		void* Data() { return m_Data; }
		const void* Data() const { return static_cast<const void*>(m_Data); }

		template<typename T>
		T& Read(uint64_t offset = 0)
		{ return *(T*)((uint8_t*)m_Data + offset); }
		template<typename T>
		const T& Read(uint64_t offset = 0) const
		{ return *(T*)((uint8_t*)m_Data + offset); }

		void Write(const void* data, uint64_t size, uint64_t offset = 0);

		template<typename T>
		void SetObjectPtr(T* ptr)
		{
			m_Data = static_cast<void*>(ptr);
			m_Size = sizeof(T);
		}


		uint64_t GetSize() const { return m_Size; }

		operator bool() const { return m_Data; }
		uint8_t& operator[](uint64_t index) { return ((uint8_t*)m_Data)[index]; }
		uint8_t operator[](uint64_t index) const { return ((uint8_t*)m_Data)[index]; }

		template<typename T>
		T* As() const { return (T*)m_Data; }

	private:
		void* m_Data{};
		uint64_t m_Size{};
	};
}