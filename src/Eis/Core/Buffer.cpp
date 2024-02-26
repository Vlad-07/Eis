#include <Eispch.h>
#include "Buffer.h"
#include "Core.h"

Eis::Buffer::Buffer(const void* data, uint64_t size) : m_Data(nullptr), m_Size(size)
{
	if (size == 0) return;
 	Allocate(size);
	Write(data, size);
}

Eis::Buffer::Buffer(const Buffer& buf) : m_Data(nullptr), m_Size(buf.m_Size)
{
	if (m_Size == 0) return;
	Allocate(m_Size);
	Write(buf.m_Data, buf.m_Size);
}

Eis::Buffer::Buffer(Buffer&& buf) : m_Data(buf.m_Data), m_Size(buf.m_Size)
{
	buf.Release();
}


void Eis::Buffer::Allocate(uint64_t size)
{
	delete[](uint8_t*)m_Data;
	m_Data = nullptr;
	m_Size = size;

	if (m_Size == 0)
		return;

	m_Data = new uint8_t[size];
}

void Eis::Buffer::Release()
{
	delete[](uint8_t*)m_Data;
	m_Data = nullptr;
	m_Size = 0;
}

void Eis::Buffer::ZeroInit()
{
	memset(m_Data, 0, m_Size);
}

template<typename T>
inline T& Eis::Buffer::Read(uint64_t offset)
{
	return *(T*)((uint32_t*)m_Data + offset);
}
template<typename T>
inline T Eis::Buffer::Read(uint64_t offset) const
{
	return *(T*)((uint32_t*)m_Data + offset);
}

void Eis::Buffer::Write(const void* data, uint64_t size, uint64_t offset)
{
	EIS_CORE_ASSERT(size + offset <= m_Size, "Buffer overflow!");
	memcpy((uint8_t*)m_Data + offset, data, size);
}