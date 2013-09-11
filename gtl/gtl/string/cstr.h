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

#ifndef GTL_STRING_CSTR_H
#define GTL_STRING_CSTR_H

#include <gtl/common.h>
#include <cstring>
#include <ctype.h>

namespace gtl { namespace string {

extern int snprintf(char* buffer, size_t count, char const* format, ...);
extern int snprintf(wchar_t* buffer, size_t count, wchar_t const* format, ...);

inline size_t strlen(char const* str)
{
	return ::strlen(str);
}

inline size_t strlen(wchar_t const* str)
{
	return ::wcslen(str);
}

/*
    4  * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
    5  *
    6  * Permission to use, copy, modify, and distribute this software for any
    7  * purpose with or without fee is hereby granted, provided that the above
    8  * copyright notice and this permission notice appear in all copies.
    9  *
   10  * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   11  * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   12  * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   13  * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   14  * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   15  * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   16  * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
   17  */

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
template <class char_t>
size_t strlcpy(char_t *dst, const char_t *src, size_t siz)
{
	char_t *d = dst;
	const char_t *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
template <class char_t>
size_t
strlcat(char_t *dst, const char_t *src, size_t siz)
{
	char_t *d = dst;
	const char_t *s = src;
	size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (*d != '\0' && n-- != 0)
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

}} //ns

#endif
