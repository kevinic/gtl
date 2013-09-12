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
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef GTL_CONTAINERS_VECTOR_H
#define GTL_CONTAINERS_VECTOR_H

#include <gtl/common.h>
#include <gtl/context.h>
#include <gtl/range.h>
#include <iterator>
#include "algorithm.h"
#include "construct.h"
#include "emplace.h"

namespace gtl {

template <class T> struct Vector_Storage
{
	Vector_Storage(Context const* context) :
		m_context(context),
		m_start(0), m_finish(0), m_end(0)
	{
	}

	Vector_Storage(Context const* context, size_t n) :
		m_context(context),
		m_start(0), m_finish(0), m_end(0)
	{
		m_start = allocate(n);
		m_finish = m_start;
		m_end = m_start + n;
	}

	~Vector_Storage()
	{
		deallocate(m_start);
	}

	T* allocate(size_t n)
	{
		return static_cast<T*>(m_context->allocator->allocate(sizeof(T) * n));
	}

	void deallocate(T* data)
	{
		m_context->allocator->deallocate(data);
	}

	Context const* m_context;
	T* m_start;
	T* m_finish;
	T* m_end;
};

struct Empty_Storage_Tag {};

template <class T> class Vector : private Vector_Storage<T>
{
	typedef Vector_Storage<T> storage_type;

public:
	typedef T value_type;
	typedef T* iterator;
	typedef T const* const_iterator;
	typedef value_type& reference;
	typedef value_type const& const_reference;
	typedef Iterator_Range<T*> range;
	typedef Iterator_Range<T const*> const_range;

	Vector(Context const* context) : storage_type(context)
	{
	}

	Vector(Context const* context, size_t count) :
		storage_type(context, count)
	{
		this->m_finish = this->m_start + count;
		construct_range(all());
	}

	Vector(Context const* context, size_t count, Empty_Storage_Tag const&) :
		storage_type(context, count)
	{
	}

	Vector(Vector const& other) :
		storage_type(other.m_context, other.size())
	{
		this->m_finish = this->m_start + other.size();
		uninitialized_copy(other.m_start, other.m_end, this->m_start);
	}

	~Vector()
	{
		destruct_range(all());
	}

	Vector& operator=(const Vector& x);

	iterator begin() {return this->m_start;}
	const_iterator begin() const {return this->m_start;}

	iterator end() {return this->m_finish;}
	const_iterator end() const {return this->m_finish;}

	range all() {return range(this->m_start, this->m_finish);}
	const_range all() const {return const_range(this->m_start, this->m_finish);}

	T& operator[](size_t index) { return this->m_start[index];}
	T const& operator[](size_t index) const { return this->m_start[index];}

	size_t size() const {return static_cast<size_t>(this->m_finish - this->m_start);}
	size_t capacity() const {return static_cast<size_t>(this->m_end - this->m_start);}
	bool empty() const {return this->m_start == this->m_finish;}

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	void push_back(const value_type& x)
	{
		func_emplace_back(gtl::emplace(x));
	}

	template <class TT>
	void emplace_back(TT&& x)
	{
		func_emplace_back(gtl::emplace(x));
	}

	template <class Func_T>
	void func_emplace_back(Func_T emplace_func)
	{
		if (this->m_finish != this->m_end)
		{
			emplace_func(this->m_finish);
			++this->m_finish;
		}
		else
		{
			insert_aux(end(), emplace_func);
		}
	}

	void fixed_push_back(const value_type& x)
	{
		fixed_func_emplace_back(gtl::emplace(x));
	}

	template <class TT>
	void fixed_emplace_back(TT&& x)
	{
		fixed_func_emplace_back(gtl::emplace(x));
	}

	template <class Func_T>
	void fixed_func_emplace_back(Func_T emplace_func)
	{
		GTL_ASSERT(this->m_finish != this->m_end);
		emplace_func(this->m_finish);
		++this->m_finish;
	}

	iterator insert(iterator position, const T& x) 
	{
		return func_emplace(position, gtl::emplace(x));
	}

	template <class TT>
	iterator emplace(iterator position, TT&& x)
	{
		return func_emplace(position, gtl::emplace(x));
	}

	template <class Func_T>
	iterator func_emplace(iterator position, Func_T func)
	{
		size_t n = position - begin();
		if (this->m_finish != this->m_end && position == end())
		{
			func(this->m_finish);
			++this->m_finish;
		}
		else
		{
			insert_aux(position, func);
		}
		return begin() + n;
	}

	template <class Input_Iterator>
	void insert(iterator position, Input_Iterator first, Input_Iterator last)
	{
		range_insert(position, first, last,
			typename std::iterator_traits<Input_Iterator>::iterator_category());
	}

	template <class Input_Iterator>
	void assign(Input_Iterator first, Input_Iterator last)
	{
		assign_aux(first, last,
			typename std::iterator_traits<Input_Iterator>::iterator_category());
	}

	void swap(Vector& x)
	{
		std::swap(this->m_context, x.m_context);
		std::swap(this->m_start, x.m_start);
		std::swap(this->m_finish, x.m_finish);
		std::swap(this->m_end, x.m_end);
	}

	void reserve(size_t n)
	{
		if(capacity() < n)
		{
			const size_t old_size = size();
			iterator tmp = allocate_and_copy(n, this->m_start, this->m_finish);
			destruct_range(all());
			this->deallocate(this->m_start);
			this->m_start = tmp;
			this->m_finish = tmp + old_size;
			this->m_end = this->m_start + n;
		}
	}

	void pop_back()
	{
		--this->m_finish;
		destruct(this->m_finish);
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end())
		{
			copy(position + 1, this->m_finish, position);
		}
		--this->m_finish;
		destruct(this->m_finish);
		return position;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator i = copy(last, this->m_finish, first);
		destruct_range(range(i, this->m_finish));
		this->m_finish = this->m_finish - (last - first);
		return first;
	}

	void resize(size_t new_size, const T& x)
	{
		if (new_size < size()) 
		{
			erase(begin() + new_size, end());
		}
		else
		{
			fill_insert(end(), new_size - size(), x);
		}
	}

	void resize(size_t new_size) { resize(new_size, T()); }
	void clear() { erase(begin(), end()); }

private:
	template <class Forward_Iterator>
	iterator allocate_and_copy(size_t n, Forward_Iterator first, Forward_Iterator last)
	{
		auto deleter = [this](T* p){this->deallocate(p);};
		auto result = scope(this->allocate(n), deleter);
		uninitialized_copy(first, last, result.get());
		return result.release();
	}

	template <class Func_T>
	void insert_aux(iterator position, Func_T emplace_func);

	void fill_insert(iterator position, size_t n, const T& x);

	template <class Input_Iter>
	void range_insert(iterator pos, Input_Iter first, Input_Iter last, std::input_iterator_tag);
	
	template <class Forward_Iter>
	void range_insert(iterator pos, Forward_Iter first, Forward_Iter last, std::forward_iterator_tag);

	template <class Input_Iter>
	void assign_aux(Input_Iter first, Input_Iter last, std::input_iterator_tag);

	template <class Forward_Iter>
	void assign_aux(Forward_Iter first, Forward_Iter last, std::forward_iterator_tag);

private:
};

template <class T> template <class Func_T>
void Vector<T>::insert_aux(iterator position, Func_T emplace_func)
{
	if(this->m_finish != this->m_end)
	{
		copy_construct(this->m_finish, *(this->m_finish - 1));
		++this->m_finish;
		copy_backward(position, this->m_finish - 2, this->m_finish - 1);
		emplace_func(position);
	}
	else
	{
		const size_t old_size = size();
		const size_t len = old_size != 0 ? 2 * old_size : 1;

		Vector temp(this->m_context, len, Empty_Storage_Tag());

		temp.m_finish = uninitialized_copy(this->m_start, position, temp.m_start);
		emplace_func(temp.m_finish);
		++temp.m_finish;
		temp.m_finish = uninitialized_copy(position, this->m_finish, temp.m_finish);

		swap(temp);
	}
}
template <class T>
void Vector<T>::fill_insert(iterator position, size_t n, const T& x)
{
	if (n != 0)
	{
		if(static_cast<size_t>(this->m_end - this->m_finish) >= n)
		{
			T x_copy = x;
			const size_t elems_after = this->m_finish - position;
			iterator old_finish = this->m_finish;
			if (elems_after > n)
			{
				uninitialized_copy(this->m_finish - n, this->m_finish, this->m_finish);
				this->m_finish += n;
				copy_backward(position, old_finish - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else
			{
				uninitialized_fill_n(this->m_finish, n - elems_after, x_copy);
				this->m_finish += n - elems_after;
				uninitialized_copy(position, old_finish, this->m_finish);
				this->m_finish += elems_after;
				fill(position, old_finish, x_copy);
			}
		}
		else
		{
			const size_t old_size = size();        
			const size_t len = old_size + std::max(old_size, n);
			Vector temp(this->m_context, len, Empty_Storage_Tag());

			temp.m_finish = uninitialized_copy(this->m_start, position, temp.m_start);
			uninitialized_fill_n(temp.m_finish, n, x);
			temp.m_finish += n;
			temp.m_finish = uninitialized_copy(position, this->m_finish, temp.m_finish);

			swap(temp);
		}
	}
}

template <class T> template <class Input_Iter>
void Vector<T>::range_insert(
	iterator pos, 
	Input_Iter first, 
	Input_Iter last,
	std::input_iterator_tag)
{
	for( ; first != last; ++first)
	{
		pos = insert(pos, *first);
		++pos;
	}
}

template <class T> template <class Forward_Iter>
void Vector<T>::range_insert(
	iterator position,
	Forward_Iter first,
	Forward_Iter last,
	std::forward_iterator_tag)
{
	if (first != last)
	{
		size_t n = std::distance(first, last);
		if (size_t(this->m_end - this->m_finish) >= n)
		{
			const size_t elems_after = this->m_finish - position;
			iterator old_finish = this->m_finish;
			if (elems_after > n)
			{
				uninitialized_copy(this->m_finish - n, this->m_finish, this->m_finish);
				this->m_finish += n;
				copy_backward(position, old_finish - n, old_finish);
				copy(first, last, position);
			}
			else
			{
				Forward_Iter mid = first;
				std::advance(mid, elems_after);
				uninitialized_copy(mid, last, this->m_finish);
				this->m_finish += n - elems_after;
				uninitialized_copy(position, old_finish, this->m_finish);
				this->m_finish += elems_after;
				copy(first, mid, position);
			}
		}
		else
		{
			const size_t old_size = size();
			const size_t len = old_size + std::max(old_size, n);

			Vector temp(this->m_context, len, Empty_Storage_Tag());
			temp.m_finish = uninitialized_copy(this->m_start, position, temp.m_start);
			temp.m_finish = uninitialized_copy(first, last, temp.m_finish);
			temp.m_finish = uninitialized_copy(position, this->m_finish, temp.m_finish);

			swap(temp);
		}
	}
}

template <class T> template <class Input_Iter>
void Vector<T>::assign_aux(Input_Iter first, Input_Iter last, std::input_iterator_tag)
{
	iterator cur = begin();
	for ( ; first != last && cur != end(); ++cur, ++first)
		*cur = *first;
	if (first == last)
		erase(cur, end());
	else
		insert(end(), first, last);
}

template <class T> template <class Forward_Iter>
void Vector<T>::assign_aux(Forward_Iter first, Forward_Iter last, std::forward_iterator_tag)
{
	size_t len = std::distance(first, last);

	if (len > capacity())
	{
		iterator tmp = allocate_and_copy(len, first, last);
		destruct_range(all());
		this->deallocate(this->m_start);
		this->m_start = tmp;
		this->m_end = m_finish = this->m_start + len;
	}
	else if (size() >= len)
	{
		iterator new_finish = copy(first, last, this->m_start);
		destruct_range(range(new_finish, this->m_finish));
		m_finish = new_finish;
	}
	else
	{
		Forward_Iter mid = first;
		std::advance(mid, size());
		copy(first, mid, this->m_start);
		m_finish = uninitialized_copy(mid, last, m_finish);
	}
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& x)
{
	if (&x != this)
	{
		const size_t xlen = x.size();
		if (xlen > capacity())
		{
			iterator tmp = allocate_and_copy(xlen, x.begin(), x.end());
			destruct_range(all());
			this->deallocate(this->m_start);
			this->m_start = tmp;
			this->m_end = this->m_start + xlen;
		}
		else if (size() >= xlen)
		{
			iterator i = copy(x.begin(), x.end(), begin());
			destruct_range(range(i, this->m_finish));
		}
		else
		{
			copy(x.begin(), x.begin() + size(), this->m_start);
			uninitialized_copy(x.begin() + size(), x.end(), this->m_finish);
		}
		this->m_finish = this->m_start + xlen;
	}
	return *this;
}

} //namespace

#endif

