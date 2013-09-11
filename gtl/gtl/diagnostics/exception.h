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

#ifndef GTL_EXCEPTION_H
#define GTL_EXCEPTION_H

#include <gtl/common.h>
#include <gtl/string.h>

namespace gtl
{
	struct Exception
	{
		static size_t const BUFFER_SIZE = 255;

		Exception()
		{
			m_buffer[0] = 0;
		}

		Exception(char const* text)
		{
			string::strlcpy(m_buffer, text, BUFFER_SIZE);
		}

		char const* what() const {return m_buffer;};
		char m_buffer[BUFFER_SIZE];
	};

	extern void breakpoint();
	extern void log_exception(Exception const& e);
	extern void abort_exception();

	template <class Exception_T>
	inline void raise_exception(Exception_T const& e)
	{
#if GTL_EXCEPTION
		throw(e);
#else
		log_exception(e);
#	if GTL_DEBUG
		//Give the debugger a shot
		breakpoint();
#	endif
		abort_exception();
#endif
	}
}

#endif
