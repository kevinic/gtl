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

#ifndef GTL_ALLOCATOR_ALLOCATOR_H
#define GTL_ALLOCATOR_ALLOCATOR_H

#include <gtl/common.h>
#include <gtl/noncopyable.h>
#include <gtl/allocator.h>
#include <gtl/scoped.h>

namespace gtl {

//stateful allocator interface
class Allocator
{
public:
	virtual void* allocate(size_t count) = 0;
	virtual void deallocate(void* p) = 0;

protected:
	virtual ~Allocator() {}
};

} //namespace

#endif

