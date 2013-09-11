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

#ifndef GTL_RANGE_ITERATOR_RANGE_H
#define GTL_RANGE_ITERATOR_RANGE_H

#include <gtl/common.h>
#include <gtl/type_traits.h>
#include <iterator>
#include <algorithm>

namespace gtl {

template <class Iterator> class Iterator_Range
{
public:
	typedef Iterator iterator_type;
	typedef typename std::iterator_traits<iterator_type>::value_type value_type;
	typedef typename std::iterator_traits<iterator_type>::reference ref_type;
	typedef value_type const& get_type;
	typedef value_type const& set_type;

	Iterator_Range(iterator_type begin, iterator_type end) : m_begin(begin), m_end(end) {}

	template <class TT>
	Iterator_Range(Iterator_Range<TT> const& other) : m_begin(other.m_begin), m_end(other.m_end) {}

	ref_type ref() const {return *m_begin;}
	get_type get() const {return *m_begin;}

	void set(set_type value) const {*m_begin = value;}

	void pop()
	{
		++m_begin;
	}

	bool empty() const {return m_begin == m_end;}

	get_type get(size_t i) const
	{
		return *(m_begin + i);
	}

	void set(size_t i, set_type value) const
	{
		*(m_begin + i) = value;
	}

	Iterator_Range slice(size_t begin, size_t end) const
	{
		return Iterator_Range(
			m_begin + begin,
			m_begin + std::min(size(), end));
	}

	size_t size() const {return m_end - m_begin;}

	iterator_type begin() const {return m_begin;}
	iterator_type end() const {return m_end;}

	iterator_type m_begin;
	iterator_type m_end;
};

template <class T> struct is_random_range<Iterator_Range<T>>
{
	typedef typename std::is_same<
		typename std::iterator_traits<T>::iterator_category,
		std::random_access_iterator_tag
	>::type type;
};

template <class Iterator>
Iterator_Range<Iterator> make_range(Iterator begin, Iterator end)
{
	return Iterator_Range<Iterator>(begin, end);
}

template <class T, size_t N> Iterator_Range<T*> make_range(T (&a)[N])
{
	return Iterator_Range<T*>(a, a + N);
}

} //namespace

#endif

