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

#ifndef GTL_WIN_EXCEPTION_H
#define GTL_WIN_EXCEPTION_H

#include <gtl/diagnostics/exception.h>
#include <windows.h>

namespace gtl
{
	struct Win_Exception : public gtl::Exception
	{
		Win_Exception()
		{
			m_error = GetLastError();
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_error, 0, m_buffer, BUFFER_SIZE, NULL);
		}

		DWORD m_error;
	};

	inline void raise_win_exception()
	{
		Win_Exception exception;
		raise_exception(exception);
	}
}

#endif
