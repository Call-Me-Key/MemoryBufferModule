#include "MemoryBuffer.h"
#include <iostream>

MemoryBuffer::iterator::iterator(std::byte * ptr)
{
	start_end.first = ptr;
	start_end.second = start_end.first + size();
}

MemoryBuffer::iterator & MemoryBuffer::iterator::operator++()
{
	start_end.first += size();
	start_end.second = start_end.first + size();
	return *this;
}

MemoryBuffer::iterator MemoryBuffer::iterator::operator++(int)
{
	iterator old = *this;
	++*this;
	return old;
}

bool MemoryBuffer::iterator::operator==(const iterator & other) const
{
	return start_end.first == other.start_end.first;
}

bool MemoryBuffer::iterator::operator!=(const iterator & other) const
{
	return !(*this == other);
}

std::pair<std::byte*, std::byte*> MemoryBuffer::iterator::operator*()
{
	return start_end;
}

MemoryBuffer::iterator MemoryBuffer::iterator::operator[](int val)
{
	iterator it(start_end.first + sizeof(uint32_t));
	for (size_t i = 0; i < val; i++)
	{
		it++;
	}
	return it;
}

std::pair<std::byte*, std::byte*>* MemoryBuffer::iterator::operator->()
{
	return &start_end;
}

uint32_t MemoryBuffer::iterator::size()
{
	return *reinterpret_cast<uint32_t*>(start_end.first);
}

std::byte * MemoryBuffer::iterator::data()
{
	return start_end.first + sizeof(uint32_t);
}

MemoryBuffer::MemoryBuffer(int reserve_bytes_size)
{
	m_data.reserve(reserve_bytes_size);
}

MemoryBuffer::MemoryBuffer(std::vector<std::byte>&& vector) : m_data(vector)
{
}

MemoryBuffer::MemoryBuffer(const std::vector<std::byte>& vector) : m_data(vector)
{
}

MemoryBuffer::iterator MemoryBuffer::begin()
{
	return iterator(m_data.data());
}

MemoryBuffer::iterator MemoryBuffer::end()
{
	return iterator(reinterpret_cast<std::byte*>(m_data.data() + m_data.size()));
}

void MemoryBuffer::clear()
{
	m_data.clear();
}
