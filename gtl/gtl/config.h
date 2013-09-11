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

#ifndef GTL_CONFIG_H
#define GTL_CONFIG_H

#ifndef GTL_EXCEPTION
#	define GTL_EXCEPTION 0
#endif

#if GTL_DEBUG
#	define GTL_ASSERT_ENABLED 1
#else
#	define GTL_ASSERT_ENABLED 0
#endif

#if GTL_EXCEPTION
#   define GTL_TRY try
#   define GTL_CATCH_SUPPRESS catch(...) {}
#   define GTL_THROW(x) throw x
#   define GTL_RETHROW throw
#   define GTL_NOTHROW throw()
#   define GTL_UNWIND(action) catch(...) { action; throw; }
#else
#   define GTL_TRY 
#   define GTL_CATCH_SUPPRESS
#   define GTL_THROW(x) 
#   define GTL_RETHROW 
#   define GTL_NOTHROW 
#   define GTL_UNWIND(action) 
# endif

//Unchecked functions not available after vs10, thanks MS
#if defined(_MSC_VER) && _MSC_VER < 1600
#	define GTL_USE_UNCHECKED_STD
#endif

#endif
