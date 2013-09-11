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

#ifndef GTL_STRING_UTILS_H
#define GTL_STRING_UTILS_H

#include <gtl/common.h>
#include "char_range.h"

namespace gtl { namespace string {

template <class Ch>
inline Char_Range_T<Ch> str_arg(Ch* str) {return str;}

template <class Ch>
inline Char_Range_T<Ch const> str_arg(Ch const* str) {return str;}

//Assume other arguments are range types already that yield characters
template <class Range_T>
inline Range_T str_arg(Range_T str) {return str;}

template <class Str_T1, class Str_T2>
int compare(Str_T1 str1, Str_T2 str2)
{
	auto lhs(str_arg(str1));
	auto rhs(str_arg(str2));

	while(!lhs.empty() && !rhs.empty())
	{
		auto lhc = lhs.get();
		auto rhc = rhs.get();

		if(lhc != rhc)
		{
			return lhc < rhc ? -1 : 1;
		}

		lhs.pop();
		rhs.pop();
	}

	size_t size1 = lhs.size();
	size_t size2 = rhs.size();

	return size1 < size2 ? -1 : (size1 > size2 ? 1 : 0);
}

template <class Ch, class Str_T>
bool is_ch_class(Ch ch, Str_T ch_class)
{
	auto ch_range(str_arg(ch_class));

	for(;!ch_range.empty(); ch_range.pop())
	{
		if(ch == ch_range.get())
		{
			return true;
		}
	}

	return false;
}

template <class Str_T1, class Str_T2, class Ch>
void replace_ch(Str_T1 str, Str_T2 ch_class, Ch ch)
{
	auto str_range(str_arg(str));

	for(;!str_range.empty(); str_range.pop())
	{
		if(is_ch_class(str_range.get(), ch_class))
		{
			str_range.set(ch);
		}
	}
}


}} //ns

#endif

