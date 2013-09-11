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


#include "stdafx.h"
#include <crtdbg.h>
#include <windows.h>
#include <test/tests.h>

namespace gtl {

class Win_Test_Platform : public Test_Platform
{
public:
	virtual void output(char const* text)
	{
		OutputDebugStringA(text);
		OutputDebugStringA("\n");
		printf(text);
		printf("\n");
	}
};

}

int _tmain(int argc, _TCHAR* argv[])
{
		// Get current flag
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	// Turn on leak-checking bit.
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	// Set flag to the new value.
	_CrtSetDbgFlag( tmpFlag );

	//GTL_ASSERT(false);
	//gtl::raise_win32_exception();

	using namespace gtl;
	Win_Test_Platform platform;
	run_tests(platform);
	return 0;
}

