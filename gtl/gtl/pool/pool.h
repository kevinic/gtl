/**
 * Copyright 2013 Kevin Lin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GTL_POOL_POOL_H
#define GTL_POOL_POOL_H

#include <gtl/config.h>
#include <gtl/context.h>
#include <gtl/type_traits.h>
#include <gtl/containers/emplace.h>
#include <gtl/debug.h>
#include "gcd_lcm.h"

namespace gtl {

class Pool
{
public:
	Pool(Context const* context, size_t elem_size, size_t elem_align, size_t count) :
		m_context(context),
		m_pool(0),
		m_pool_end(0),
		m_free(0)
	{
		//Calculate the characteristics of the actual link
		size_t link_size = std::max(elem_size, sizeof(char*));
		size_t link_align = details::lcm(elem_align, std::alignment_of<char*>::value);

		size_t remainder = link_size % link_align;
		if(remainder != 0)
		{
			//Adjust the link size with padding
			link_size += link_align - remainder;
		}

		size_t pool_size = link_size * count;
		m_link_size = link_size;
		m_pool = static_cast<char*>(m_context->allocator->allocate(pool_size));
		m_pool_end = m_pool + pool_size;

		//Push backwards, we want to serve from the top
		char* link = m_pool_end;
		while(link > m_pool)
		{
			link = link - link_size;
			push_free(link);
		}
	}

	~Pool()
	{
		m_context->allocator->deallocate(m_pool);
	}

	size_t offset_of(void const* p) const
	{
		char const* ptr = (char const*) p;
		GTL_ASSERT(ptr >= m_pool && ptr < m_pool_end);
		return (ptr - m_pool) / m_link_size;
	}

	bool empty() const
	{
		return m_free == 0;
	}

	void* allocate()
	{
		GTL_ASSERT(!empty());

		void* ret = m_free;
		m_free = next(m_free);
		return ret;
	}

	void deallocate(void* p)
	{
		char* link = static_cast<char*>(p);
		//Must came from the pool
		GTL_ASSERT(link >= m_pool && link < m_pool_end);
		push_free(link);
	}

private:
	char* & next(char* link)
	{
		return *reinterpret_cast<char**>(link);
	}

	void push_free(char* link)
	{
		next(link) = m_free;
		m_free = link;
	}

private:
	Context const* m_context;
	size_t m_link_size;
	char* m_pool;
	char* m_pool_end;
	char* m_free;
};

template <class T> class Node_Pool
{
public:
	Node_Pool(Context const* context, size_t count) : 
		m_pool(context, sizeof(T), std::alignment_of<T>::value, count),
		m_outstanding(0)
	{
	}

	~Node_Pool()
	{
		GTL_ASSERT(m_outstanding == 0);
	}

	T* allocate()
	{
		return static_cast<T*>(m_pool.allocate());
	}

	void deallocate(T* p)
	{
		m_pool.deallocate(p);
	}

	size_t offset_of(T* p)
	{
		return m_pool.offset_of(p);
	}

	T* create()
	{
		return create(emplace());
	}

	T* create(T const& x)
	{
		return create(emplace(x));
	}

	template <class Emplace_Func>
	T* create(Emplace_Func func)
	{
		auto deleter = [this](T* p){this->deallocate(p);};
		auto result(scope(allocate(), deleter));
		func(result.get());
		++m_outstanding;
		return result.release();
	}

	void destroy(T* p)
	{
		--m_outstanding;
		destruct(p);
		m_pool.deallocate(p);
	}

	template <class Range_T>
	void destroy_range(Range_T range)
	{
		while(!range.empty())
		{
			destroy(range.front());
			range.pop_front();
		}
	}

	bool empty() const
	{
		return m_pool.empty();
	}

private:
	Pool m_pool;
	size_t m_outstanding;
};

}

#endif
