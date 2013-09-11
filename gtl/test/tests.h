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


#ifndef GTL_TEST_TESTS_H
#define GTL_TEST_TESTS_H

#include "common.h"

namespace gtl {

	extern void test_containers(Test_Platform& platform);
	extern void test_format(Test_Platform& platform);
	extern void test_pool(Test_Platform& platform);
	extern void test_string(Test_Platform& platform);
	extern void test_scoped(Test_Platform& platform);
	extern void test_range(Test_Platform& platform);
	extern void test_stream(Test_Platform& platform);

	inline void run_tests(Test_Platform& platform)
	{
		test_containers(platform);
		test_format(platform);
		test_pool(platform);
		test_string(platform);
		test_scoped(platform);
		test_range(platform);
		test_stream(platform);
	}
}

#endif