#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>

/**
Class for storing raw memory in packages.
TODO: implement input iterator 
TODO: push_back(): 
	maybe we should	forbid all 
	classes except package type.
**/
class MemoryBuffer
{
public:
	class iterator {
	public:
		//iterator traits:
		using difference_type = std::byte*;
		using value_type = std::pair<std::byte*, std::byte*>;
		using pointer = std::byte*;
		using reference = std::byte&;
		using iterator_category = std::forward_iterator_tag;

		iterator(std::byte* ptr); ///< make range iterator with the begining from "ptr"

		iterator& operator++(); ///< make a step according Size() function
		iterator operator++(int);
		bool operator==(const iterator& other) const; ///< check equealing of both begin point
		bool operator!=(const iterator& other) const; ///< check !equealing of both begin point
		std::pair<std::byte*, std::byte*> operator*(); ///< get begin and end points of the range
		iterator operator[](int val); ///< nasted iterator (insiide current range)
		std::pair<std::byte*, std::byte*>* operator->();

		uint32_t size(); ///<implement first 4 bytes of the range as range's size
		std::byte* data(); ///<ptr to memory after size and type blocks
	private:
		std::pair<std::byte*, std::byte*> start_end;
	};

	MemoryBuffer(int reserve_bytes_size = 256);
	MemoryBuffer(std::vector<std::byte>&& vector);
	MemoryBuffer(const std::vector<std::byte>& vector);

	iterator begin(); 
	iterator end(); 

	template<typename T>
	void push_back(const T& data, uint64_t size_in_bytes); ///< copy from "data" to "data" + size_in_bytes
	template<typename T>
	void push_back(const T& data);///< copy from "data" to "data" + sizeof("data")
	void clear(); ///< clear data vector
private:
	std::vector<std::byte> m_data;
};

//template<typename T>
template<typename T>
void MemoryBuffer::push_back(const T& data, uint64_t size_in_bytes)
{
	const std::byte* ptr = reinterpret_cast<const std::byte*>(&data);
	m_data.insert(m_data.end(), ptr, ptr + size_in_bytes);
}

template<typename T>
inline void MemoryBuffer::push_back(const T & data)
{
	push_back(data, sizeof(T));
}
