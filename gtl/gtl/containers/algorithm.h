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

#ifndef GTL_CONTAINERS_ALGORITHM_H
#define GTL_CONTAINERS_ALGORITHM_H

#include <gtl/common.h>
#include <algorithm>
#include <memory>

namespace gtl {

#if GTL_USE_UNCHECKED_STD
template <class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
{
	return stdext::unchecked_uninitialized_copy(first, last, dest);
}
#else
using std::uninitialized_copy;
#endif

#if GTL_USE_UNCHECKED_STD
template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator dest)
{
	return stdext::unchecked_copy(first, last, dest);
}
#else
using std::copy;
#endif

#if GTL_USE_UNCHECKED_STD
template <class BidiIter1, class BidiIter2>
BidiIter2 copy_backward(BidiIter1 first, BidiIter1 last, BidiIter2 dest)
{
	return stdext::unchecked_copy_backward(first, last, dest);
}
#else
using std::copy_backward;
#endif

using std::fill;
using std::uninitialized_fill;

#if GTL_USE_UNCHECKED_STD
template <class ForwardIterator, class Size, class Type>
void uninitialized_fill_n(ForwardIterator first, Size size, Type const& val)
{
	stdext::unchecked_uninitialized_fill_n(first, size, val);
}
#else
using std::uninitialized_fill_n;
#endif

}

#endif
