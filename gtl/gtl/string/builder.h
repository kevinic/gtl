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

#ifndef GTL_STRING_BUILDER_H
#define GTL_STRING_BUILDER_H

#include <gtl/common.h>
#include <gtl/containers/algorithm.h>

namespace gtl { namespace string {

template <class Char_T> class Static_Builder
{
public:
	typedef Char_T char_type;
	typedef Char_Range_T<Char_T> range_type;

	Static_Builder(char_type* begin, size_t count) :
		m_begin(begin),
		m_cursor(begin),
		m_end(begin + count),
		m_overflow(false)
	{
		//so we can have a valid string right away
		if(count != 0)
		{
			*m_cursor = 0;
		}
		else
		{
			m_overflow = true;
		}
	}

	char_type const* c_str() {return m_begin;}

	bool overflow() const {return m_overflow;}

	template <class Range_T>
	void append(Range_T range)
	{
		while((m_end - m_cursor > 1) && !range.empty())
		{
			*m_cursor = range.get();
			++m_cursor;
			range.pop();
		}

		*m_cursor = 0;

		if(!range.empty())
		{
			m_overflow = true;
		}
	}

	void append(char_type const* str)
	{
		return append(Char_Range_T<char_type const>(str));
	}

private:
	char_type* m_begin;
	char_type* m_end;
	char_type* m_cursor;
	bool m_overflow;
};


template <class Char_T, size_t N>
Static_Builder<Char_T> make_builder(Char_T (&str)[N])
{
	return Static_Builder<Char_T>(str, N);
}



}} //ns

#endif

