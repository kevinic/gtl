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

#ifndef GTL_FORMAT_FORMAT_H
#define GTL_FORMAT_FORMAT_H

#include <gtl/common.h>

namespace gtl { namespace format {

static size_t const MAX_ARG = 10;

class Variant
{
public:
	union storage_type
	{
		int i;
		unsigned int ui;
		float f;
		char const* str;
		wchar_t const* wstr;
		void* p;
	};

	Variant() {}
	Variant(int i) {m_storage.i = i;}
	Variant(unsigned int ui) {m_storage.ui = ui;}
	Variant(float f) {m_storage.f =  f;}
	Variant(char const* str) {m_storage.str = str;}
	Variant(wchar_t const* wstr) {m_storage.wstr = wstr;}
	Variant(void* p) {m_storage.p = p;}

	storage_type get() const {return m_storage;}

private:
	storage_type m_storage;
};

//Really simple class for capturing printf variable args.  Doesn't screw
//around with template magic, just stores an array of pre-defined union.
class Expression
{
public:
	Expression(char const* fmt) : m_fmt(fmt), m_count(0) {}

	template <class T> Expression& with(T const& arg)
	{
		GTL_ASSERT(m_count < MAX_ARG);
		m_args[m_count] = Variant(arg);
		++m_count;
		return *this;
	}

	Variant::storage_type get(size_t i) const
	{
		return m_args[i].get();
	}

	char const* fmt() const {return m_fmt;}

private:
	char const* m_fmt;
	Variant m_args[MAX_ARG];
	size_t m_count;
};

//short-hand
inline
Expression fmt(char const* fmt_str)
{
	return Expression(fmt_str);
}

template <class T>
inline Expression& operator%(Expression& expr, T const& arg)
{
	return expr.with(arg);
}

}}

#endif

