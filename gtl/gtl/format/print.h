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

#ifndef GTL_FORMAT_PRINT_H
#define GTL_FORMAT_PRINT_H

#include <gtl/common.h>
#include <gtl/string.h>
#include "format.h"

namespace gtl { namespace format {

inline
int snprint(char* buffer, size_t count, Expression const& expr)
{
	static_assert(MAX_ARG == 10, "unsupported arg size");
	return string::snprintf(buffer, count, expr.fmt(),
		expr.get(0),
		expr.get(1),
		expr.get(2),
		expr.get(3),
		expr.get(4),
		expr.get(5),
		expr.get(6),
		expr.get(7),
		expr.get(8),
		expr.get(9)
	);
}

}}

#endif


