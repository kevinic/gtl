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

#ifndef GTL_STRING_CHAR_RANGE_H
#define GTL_STRING_CHAR_RANGE_H

#include <gtl/common.h>
#include <gtl/containers/algorithm.h>
#include <gtl/range.h>
#include <gtl/type_traits.h>
#include "cstr.h"

namespace gtl { namespace string {

template <class Char> class Char_Range_T
{
public:
	typedef Char char_type;
	typedef char_type get_type;
	typedef char_type set_type;
	typedef typename std::remove_cv<char_type>::type true_char_type;

	Char_Range_T() : m_begin(0), m_end(0) {}

	Char_Range_T(char_type* begin, char_type* end) :
		m_begin(begin), m_end(end) {}

	Char_Range_T(char_type* str) :
		m_begin(str), m_end(str + strlen(str))
	{}

	Char_Range_T(char_type* str, size_t n) :
		m_begin(str), m_end(str + n)
	{}

	Char_Range_T(Char_Range_T<true_char_type> const& range) :
		m_begin(range.begin()), m_end(range.end()) {}

	get_type get() const {return *m_begin;}
	void set(set_type ch) const {*m_begin = ch;}

	get_type get(size_t i) const {return m_begin[i];}
	void set(size_t i, set_type ch) const {m_begin[i] = ch;}

	void pop() {++m_begin;}

	bool empty() const {return m_end == m_begin;}
	size_t size() const {return m_end - m_begin;}

	char_type* begin() const {return m_begin;}
	char_type* end() const {return m_end;}

	Char_Range_T slice(size_t begin, size_t end)
	{
		return Char_Range_T(
			m_begin + begin,
			m_begin + std::min(size(), end));
	}

	int compare(Char_Range_T const& other)
	{
		return gtl::string::compare(*this, other);
	}

	size_t find(Char_Range_T const& text, size_t pos = 0)
	{
		if(pos + text.size() > size())
		{
			return size();
		}
		else
		{
			char_type* result = std::search(m_begin, m_end, text.m_begin, text.m_end);
			return result - m_begin;
		}
	}

private:
	char_type* m_begin;
	char_type* m_end;
};

template <class Char> struct is_random_range<Char_Range_T<Char>>
{
	typedef true_type type;
};

typedef Char_Range_T<char> Char_Range;
typedef Char_Range_T<char const> Char_Const_Range;
typedef Char_Range_T<wchar_t> Wchar_Range;
typedef Char_Range_T<wchar_t const> Wchar_Const_Range;

//some specializations
inline
int compare(Char_Const_Range const& lhs, Char_Const_Range const& rhs)
{
	size_t size1 = lhs.size();
	size_t size2 = rhs.size();
	int comp = memcmp(lhs.begin(), rhs.begin(), std::min(size1, size2));
	return comp ? comp : (size1 < size2 ? -1 : (size1 > size2 ? 1 : 0));
}

}} //ns

#endif
