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

#ifndef GTL_DIAGNOSTICS_ASSERT_H
#define GTL_DIAGNOSTICS_ASSERT_H

#include <gtl/config.h>
#include "current_function.h"

namespace gtl
{
	extern void assertion_failed(char const * expr, char const * function, char const * file, long line);
}

#if GTL_ASSERT_ENABLED
#	define GTL_ASSERT(expr) ((expr)? ((void)0): ::gtl::assertion_failed(#expr, GTL_CURRENT_FUNCTION, __FILE__, __LINE__))
#else
#	define GTL_ASSERT(expr) ((void)0)
#endif

#endif
