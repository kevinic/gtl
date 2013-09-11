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

#include <gtl/common.h>
#include <crtdbg.h>
#include <intrin.h>
#include <windows.h>
#include <stdlib.h>
#include <gtl/diagnostics/assert.h>
#include <gtl/diagnostics/exception.h>

namespace gtl
{
	//Default implementation for assertion fail on win32
	extern void assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		GTL_REF(expr);
		GTL_REF(function);
		GTL_REF(file);
		GTL_REF(line);

#if _DEBUG
		//For debug CRT use the crt report
		_RPT4(_CRT_ASSERT, "\"%s\" at %s, %s(%i)", expr, function, file, line);
#else
		//For release mode, pretend a break point happened
		__debugbreak();
#endif
	}

	extern void breakpoint()
	{
		__debugbreak();
	}

	extern void log_exception(Exception const& e)
	{
		//Crude message box
		MessageBoxExA(NULL, e.what(), NULL, MB_OK, 0);
	}

	extern void abort_exception()
	{
		abort();
	}
}
