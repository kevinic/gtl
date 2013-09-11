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


#include "common.h"
#include <gtl/format.h>

namespace gtl {

class Test_Format : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{
		using namespace gtl::format;

		char buffer[100];

		void* p = 0;

		snprint(buffer, 100,
			fmt("test %u %i %s 0x%x") % 10u % 20 % "test" % p);

		int ret = strcmp(buffer, "test 10 20 test 0x0");

		GTL_TEST_VERIFY(tc, !ret);
	}
};

void test_format(Test_Platform& platform)
{
	Test_Suite suite("format", platform);

	Test_Format test_construct;
	suite.run("format", test_construct);
}

} //ns
