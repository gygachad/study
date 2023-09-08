#pragma once

#include <list>
#include <cassert>
#include <cstring>
#include <inttypes.h>
#include <memory>

#include "debug.h"

using namespace std;

struct mem_block
{
	bool* m_alloc_bytemap	= nullptr;
	byte* m_mem_pool_start	= nullptr;
	byte* m_mem_pool_end	= nullptr;

	size_t m_item_size			= 0x0;
	size_t m_free_items_count	= 0x0;
	size_t m_reserve_items		= 0x0;
	size_t m_last_free_pos		= 0x0;
	size_t m_mem_allocated		= 0x0;

	mem_block(size_t item_size, size_t reserve_items = 0x100)
	{
		m_reserve_items = reserve_items;
		m_item_size = item_size;

		m_free_items_count = reserve_items;

		m_alloc_bytemap = (bool*)malloc(sizeof(bool) * reserve_items);
		m_mem_pool_start = (byte*)malloc(m_item_size * reserve_items);

		if (!m_alloc_bytemap || !m_mem_pool_start)
			throw bad_alloc();

		m_mem_allocated = m_item_size * reserve_items;
		m_mem_pool_end = m_mem_pool_start + m_mem_allocated;

		memset(m_alloc_bytemap, 0x0, sizeof(bool) * reserve_items);
		memset(m_mem_pool_start, 0x0, m_item_size * reserve_items);

		DEBUG_MSG("mem_block [" << this << "] -> m_mem_pool  [" << m_mem_pool_start << "]" << endl);
	}

	mem_block(const mem_block& other) = delete;

	~mem_block()
	{
		free(m_alloc_bytemap);
		free(m_mem_pool_start);

		DEBUG_MSG("~mem_block() [" << this << "] -> free(m_mem_pool) [" << m_mem_pool_start << "]" << endl);
	}


	byte* allocate(size_t n)
	{
		byte* p = nullptr;
		
		size_t start_pos = 0x0;

		//If we have enough items in the end - start with it
		if (m_reserve_items - m_last_free_pos >= n)
			start_pos = m_last_free_pos;

		//Try to find sequence of n free items
		for (size_t i = start_pos; i <= m_reserve_items - n; i++)
		{
			bool success = true;

			for (size_t j = i; j < i + n; j++)
			{
				//This item already allocated. Try another one
				if (m_alloc_bytemap[j])
				{
					success = false;
					break;
				}
			}

			if (success)
			{
				size_t next_free_pos = 0;
				//Now they are allocated
				for (size_t j = i; j < i + n; j++)
				{
					m_alloc_bytemap[j] = true;
					next_free_pos = j + 1;
				}

				if (i + n == m_reserve_items)
					next_free_pos = 0;

				//Set next free item position
				m_last_free_pos = next_free_pos;

				//Calculate start address of allocated sequnce
				p = (m_mem_pool_start + i * m_item_size);

				m_free_items_count -= n;

				DEBUG_MSG("mem_block [" << this << "] -> allocate " << n << "" << endl);

				break;
			}
		}

		DEBUG_MSG("mem_block [" << this << "] -> allocate [" << p << "] from m_mem_pool [" << m_mem_pool_start << "]" << std::endl);

		return p;
	}

	bool deallocate(byte* p, size_t n)
	{		
		//Let's check - if this item from another mem_block with same m_item_size
		if (m_mem_pool_start <= p &&
			p < m_mem_pool_end)
		{
			uint64_t pos = (p - m_mem_pool_start)/m_item_size;
			for (size_t i = 0; i < n; i++)
			{
				m_alloc_bytemap[pos] = false;
			}

			m_last_free_pos = pos;
			m_free_items_count += n;

			return true;
		}

		DEBUG_MSG("deallocate " << p << " from pool " << m_mem_pool_start << std::endl);

		return false;
	}
};

template<size_t reserved>
class mem_pool
{
	static_assert(reserved != 0, "Reserved can't be 0");

	list<mem_block*> m_block_list;
	size_t m_reserved_items = reserved;

public:
	mem_pool(){}
	mem_pool(const mem_pool& other) = delete;

	~mem_pool() 
	{
		for (mem_block* block : m_block_list)
			delete block;

		m_block_list.clear();
	}

	template <typename T>
	T* allocate(size_t n)
	{
		T* p = nullptr;

		for (mem_block* block : m_block_list)
		{
			//Try find block for item size
			if (sizeof(T) != block->m_item_size)
				continue;

			//This block is full of items
			if (block->m_free_items_count == 0x0 || n > block->m_reserve_items)
				continue;

			p = reinterpret_cast<T*>(block->allocate(n));

			//Item allocated
			if (p)
				break;
		}

		//It's possible if block does't contain sequnce of n items
		//Fragmentation
		if (!p)
		{
			//Add new mem_block. Check - if requsted n bigger then reserved
			mem_block* block = new mem_block(sizeof(T), n <= m_reserved_items ? m_reserved_items : n);

			//First block for container proxy - put it in the end
			m_block_list.push_front(block);

			p = reinterpret_cast<T*>(block->allocate(n));

			assert(p);
		}

		return p;
	}

	template <typename T>
	void deallocate(T* p, size_t n)
	{
		mem_block* block_for_delete = nullptr;

		for (mem_block* block : m_block_list)
		{
			if (sizeof(T) == block->m_item_size)
			{
				if (!block->deallocate(reinterpret_cast<byte*>(p), n))
				{
					//This pointer from another mem_block with same size
					//Try another one
					continue;
				}
				else
				{
					//Check for block empty
					if (block->m_free_items_count == block->m_reserve_items)
						block_for_delete = block;

					//n items successfully deallocated
					break;
				}
			}
		}

		//Free this mem_block. It's empty now
		if (block_for_delete)
		{
			m_block_list.remove(block_for_delete);
			delete block_for_delete;
		}
	}
};

template <typename T>
class linear_allocator
{
	//const size_t m_reserved = 0x100;

public:
	shared_ptr<mem_pool<0x1000>> m_shared_mem_pool = make_shared<mem_pool<0x1000>>();

	using value_type = T;

	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	
	linear_allocator(){}

	~linear_allocator(){}

	template <typename U>
	linear_allocator(const linear_allocator<U>& other)
	{
		m_shared_mem_pool = other.m_shared_mem_pool;
	}

	template<typename U>
	constexpr bool operator== (const linear_allocator<U>&)
	{ 
		return true;
	}

	template<typename U>
	constexpr bool operator!= (const linear_allocator<U>&)
	{
		return false;
	}

	T* allocate(size_t n)
	{
		T* p = m_shared_mem_pool->allocate<T>(n);

		return p;
	}

	void deallocate(T* p, size_t n)
	{
		m_shared_mem_pool->deallocate(p, n);
	}

	template <typename U, typename... Args>
	void construct(U* p, Args &&...args)
	{
		new (p) U(std::forward<Args>(args)...);
	}

	template <typename U>
	void destroy(U* p)
	{
		p->~U();
	}
};