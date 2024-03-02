#pragma once

namespace Eis
{
	// Dynamically allocated buffer
	class Buffer
	{
	public:
		Buffer() : m_Data(nullptr), m_Size(0) {}
		Buffer(const void* data, uint64_t size);
		Buffer(const Buffer& buf);
		Buffer(Buffer&& buf);
		~Buffer() { Release(); }

		void Allocate(uint64_t size);
		void Resize(uint64_t size);
		void Release();

		void ZeroInit();
		void NullTerminate();

		void* Data() const { return m_Data; }

		template<typename T>
		T& Read(uint64_t offset = 0);
		template<typename T>
		T Read(uint64_t offset = 0) const;

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