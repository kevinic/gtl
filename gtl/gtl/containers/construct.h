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

#ifndef GTL_CONTAINERS_CONSTRUCT_H
#define GTL_CONTAINERS_CONSTRUCT_H

#include <gtl/common.h>
#include <gtl/diagnostics/exception.h>
#include <gtl/type_traits.h>
#include <gtl/range.h>
#include <iterator>

namespace gtl {

template <class T>
inline void construct_aux(T* /*pT*/, true_type /*trivial*/)
{
}

template <class T>
inline void construct_aux(T* pT, false_type /*trivial*/)
{
	new (pT) T();
}

template <class T>
inline void construct(T* pT)
{
	construct_aux(pT, typename std::has_trivial_default_constructor<T>::type());
}

template <class T>
inline void copy_construct(T* pT, T const& x)
{
	new (pT) T(x);
}

template <class T>
inline void destruct_aux(T* /*pT*/, true_type /*trivial*/)
{
}

template <class T>
inline void destruct_aux(T* pT, false_type /*trivial*/)
{
	pT->~T();
}

template <class T>
inline void destruct(T* pT)
{
	destruct_aux(pT, typename std::has_trivial_destructor<T>::type());
}

template <class Range_T>
void destruct_range(Range_T range)
{
	for(; !range.empty(); range.pop())
	{
		destruct(&range.ref());
	}
}

template <class Range_T>
void construct_range_aux(Range_T range, true_type /*no_throw*/)
{
	for(; !range.empty(); range.pop())
	{
		construct(&range.ref());
	}
}

#if GTL_EXCEPTION
template <class Range_T>
void construct_range_aux(Range_T range, false_type /*no_throw*/)
{
	Range_T saved = range;
	size_t n = 0;

	try
	{
		for(; !range.empty(); range.pop())
		{
			construct(&range.ref());
			++n;
		}
	}
	catch(...)
	{
		destruct_range(truncate_range(saved, n));
		throw;
	}
}

template <class Range_T>
inline void construct_range(Range_T range)
{
	typedef std::remove_cv<typename Range_T::get_type>::type value_type;
	construct_range_aux(
		range, std::has_nothrow_constructor<value_type>::type());
}

template <class Range_T, class Func_T>
void emplace_construct_range(Range_T range, Func_T func)
{
	Range_T saved = range;
	size_t n = 0;

	try
	{
		for(; !range.empty(); range.pop())
		{
			func(&range.ref());
			++n;
		}
	}
	catch(...)
	{
		destruct_range(truncate_range(saved, n));
		throw;
	}
}

#else //GTL_EXCEPTION

template <class Range_T>
inline void construct_range(Range_T range)
{
	construct_range_aux(range, true_type());
}

template <class Range_T, class Func_T>
void emplace_construct_range(Range_T range, Func_T func)
{
	for(; !range.empty(); range.pop())
	{
		func(&range.ref());
	}
}

#endif

} //namespace

#endif

