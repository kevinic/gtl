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

#ifndef GTL_CONTEXT_H
#define GTL_CONTEXT_H

#include "common.h"
#include "allocator.h"
#include "debug.h"
#include "containers/construct.h"

namespace gtl {

struct Context
{
	Context(Allocator* allocator) : allocator(allocator)
	{
	}

	//p must be the concrete type that was allocated! (no virtual deallocation).
	//creator frees is the GTL policy
	template <class T> void destroy(T* p) const
	{
		destruct(p);
		allocator->deallocate(p);
	}
	
	Allocator* allocator;
};

} //ns

inline void* operator new(size_t count, gtl::Context const* context)
{
	return context->allocator->allocate(count);
}

//placement override for exception safety
inline void operator delete(void* p, gtl::Context const* context)
{
	context->allocator->deallocate(p);
}

#endif

