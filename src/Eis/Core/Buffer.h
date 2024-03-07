#pragma once

namespace Eis
{
	class Buffer
	{
	public:
		Buffer() noexcept : m_Data(nullptr), m_Size(0) {}
		Buffer(const void* data, uint64_t size) noexcept;
		Buffer(const Buffer& buf) noexcept;
		Buffer(Buffer&& buf) noexcept;
		~Buffer() noexcept;

		void Allocate(uint64_t size);
		void Resize(uint64_t size);
		void Release();

		void ZeroInit();
		void AppendNull();

		void* Data() const { return m_Data; }

		template<typename T>
		T& Read(uint64_t offset = 0)
		{ return *(T*)((uint8_t*)m_Data + offset); }
		template<typename T>
		T Read(uint64_t offset = 0) const
		{ return *(T*)((uint8_t*)m_Data + offset); }

		void Write(const void* data, uint64_t size, uint64_t offset = 0);


		uint64_t GetSize() const { return m_Size; }

		operator bool() const { return m_Data; }
		uint8_t& operator[](uint64_t index) { return ((uint8_t*)m_Data)[index]; }
		uint8_t operator[](uint64_t index) const { return ((uint8_t*)m_Data)[index]; }

		template<typename T>
		T* As() const { return (T*)m_Data; }

	private:
		void* m_Data;
		uint64_t m_Size;
	};
}