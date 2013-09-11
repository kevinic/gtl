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

#ifndef GTL_RANGE_ALGORITHM_H
#define GTL_RANGE_ALGORITHM_H

#include "range.h"
#include "detail/range_adapter.h"

namespace gtl { 

	namespace detail {

template <class T>
Truncate_Range<T> truncate_range_aux(T const& range, size_t n, false_type /*random range*/)
{
	return Truncate_Range<T>(range, n);
}

template <class T>
T truncate_range_aux(T const& range, size_t n, true_type /*random range*/)
{
	return range.slice(0, n);
}

template <class Range_T>
Random_Slice_Range<Range_T> slice_aux(
	Range_T const& range, size_t begin, size_t end, size_t step, true_type /*random*/)
{
	return Random_Slice_Range<Range_T>(range, begin, end, step);
}

template <class Range_T>
Slice_Range<Range_T> slice_aux(
	Range_T const& range, size_t begin, size_t end, size_t step, false_type /*random*/)
{
	return Slice_Range<Range_T>(range, begin, end, step);
}

	} //detail

template <class T>
auto truncate_range(T const& range, size_t n) ->
	decltype(detail::truncate_range_aux(range, n, is_random_range<T>::type()))
{
	return detail::truncate_range_aux(range, n, is_random_range<T>::type());
}

template <class Range_T>
auto slice(Range_T const& range, size_t begin, size_t end = range.size(), size_t step = 1)
	-> decltype(detail::slice_aux(range, begin, end, step, typename is_random_range<Range_T>::type()))
{
	return detail::slice_aux(range, begin, end, step, typename is_random_range<Range_T>::type());
}

} //gtl ns

#endif
