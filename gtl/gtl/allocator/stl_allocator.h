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

#ifndef GTL_ALLOCATOR_STL_ALLOCATOR_H
#define GTL_ALLOCATOR_STL_ALLOCATOR_H

#include <memory>
#include <gtl/config.h>
#include "allocator.h"

namespace gtl {

template <class T> class STL_Allocator;

template <> class STL_Allocator<void>
{
public:
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef void value_type;
	template <class U> struct rebind
	{
		typedef STL_Allocator<U> other;
	};

	STL_Allocator(Allocator& alloc) : m_alloc(&alloc) {}

	STL_Allocator(STL_Allocator const& other) throw() :
		m_alloc(other.m_alloc)
	{
	}

	template <class U> STL_Allocator(const STL_Allocator<U>& other) throw() :
		m_alloc(other.m_alloc)
	{
	}

	gtl::Allocator* m_alloc;
};

template <class T> class STL_Allocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	template <class U> struct rebind
	{
		typedef STL_Allocator<U> other;
	};

	STL_Allocator(Allocator& alloc) : m_alloc(&alloc) {}

	STL_Allocator(STL_Allocator const& other) throw() :
		m_alloc(other.m_alloc)
	{
	}

	template <class U> STL_Allocator(const STL_Allocator<U>& other) throw() :
		m_alloc(other.m_alloc)
	{
	}

	~STL_Allocator() throw()
	{
	}

	pointer address(reference x) const
	{
		return &x;
	}

	const_pointer address(const_reference x) const
	{
		return &x;
	}

	pointer allocate(size_type count,
		STL_Allocator<void>::const_pointer = 0)
	{
		return static_cast<pointer>(m_alloc->allocate(sizeof(T) * count));
	}

	void deallocate(pointer p, size_type)
	{
		m_alloc->deallocate(p);
	}

	size_type max_size() const throw()
	{
		return static_cast<size_type>(-1);
	}

	void construct(pointer p, const T& val)
	{
		new (p) T(val);
	}

	void destroy(pointer p)
	{
		(void) p; //unused reference
		p->~T();
	}

	gtl::Allocator* m_alloc;
};

template <class U, class T>
inline bool operator==(STL_Allocator<U> const& lhs, STL_Allocator<T> const& rhs)
{
	return lhs.m_alloc == rhs.m_alloc;
}

template <class U, class T>
inline bool operator!=(STL_Allocator<U> const& lhs, STL_Allocator<T> const& rhs)
{
	return lhs.m_alloc != rhs.m_alloc;
}

} //gtl

#endif
