/*
 * path.c -- path tools
 * Copyright (C) 2021  Jacob Koziej <jacobkoziej@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "path.h"

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"


/*
 * Return the full file path.
 */
char *path_full(char *path, char *wd)
{
	/*
	 * The existing implementation of realpath() in glibc dereferences
	 * symlinks by default and makes sure all elements exist.
	 *
	 * We don't want that.  Instead we want an absolute path to the input
	 * relative path clean of unnecessary characters regardless of if it
	 * exists and/or consists of symlinks.
	 */

	assert(path);
	assert(wd);


	char *buf, *nil, *buf_lim = NULL;
	char *head, *tail = NULL;
	size_t buf_siz = strlen(wd) + 1 + PATH_MAX;


	if (*path == '\0') return NULL;


	buf = calloc(buf_siz, sizeof(char));
	if (!buf) return NULL;

	strcpy(buf, wd);
	nil = buf + buf_siz - PATH_MAX - 1;
	buf_lim = buf + buf_siz;


	for (tail = head = path; *head; head = tail) {
		// skip multiple '/'
		while (*head == '/') ++head;

		// find the end of the path component
		tail = head;
		while (*tail && *tail != '/') ++tail;

		// manage relative components
		uintptr_t diff = tail - head;
		if (diff == 0) break;
		if (diff == 1 && head[0] == '.') continue;
		if (diff == 2 && head[0] == '.' && head[1] == '.') {
			// go up to the previous path component (ignoring root)
			if (nil > buf + 1) {
				while ((--nil)[-1] != '/');
				*nil = '\0';
			}
			continue;
		}

		// make sure we have a trailing '/'
		if (nil[-1] != '/') *nil++ = '/';

		// resize if necessary
		if (nil + diff >= buf_lim) {
			uintptr_t nil_offset = nil - buf;
			char *buf_new = NULL;

			buf_siz += (diff + 1 > PATH_MAX) ? diff + 1 : PATH_MAX;

			buf_new = realloc(buf, buf_siz);
			if (!buf_new) goto error;

			buf = buf_new;
			buf_lim = buf + buf_siz;
			nil = buf + nil_offset;
		}

		nil = mempcpy(nil, head, diff);
		*nil = '\0';
	}

	return buf;

error:
	FREE(buf);
	return NULL;
}

/*
 * Generate a relative path from an absolute source to an absolute destination.
 */
char *path_relative(char *src, char *dst)
{
	assert(src);
	assert(dst);


	char   *buf     = NULL;
	int     dir_lvl = 0;
	size_t  src_len = 0;
	char   *start   = src;  // so we don't backtrack past our start


	// remove the shared path
	while ((*src == *dst) && *src && *dst) {
		++src; ++dst;
	}

	// strip only up only up to the directory name
	while ((*src != '/') && (src >= start)) {
		--src; --dst;
	}

	// strip '/'
	++src; ++dst;

	src_len = strlen(src);
	for (size_t i = 0; i < src_len; i++) {
		if (src[i] == '/') ++dir_lvl;
	}


	buf = calloc(
		strlen(dst) + (strlen("../") * dir_lvl) + 1,
		sizeof(char)
	);
	if (!buf) return NULL;

	for (int i = 0; i < dir_lvl; i++) {
		strcat(buf, "../");
	}
	strcat(buf, dst);


	return buf;
}

/*
 * Substitute a pattern in a path with a substring.
 */
char *path_substitution(char *path, char *pat, char *sub)
{
	assert(path);
	assert(pat);
	assert(sub);


	char   *buf     = NULL;
	size_t  offset  = 0;
	size_t  pat_len = strlen(pat);


	// ensure pattern matches path
	while (offset < pat_len) {
		if (path[offset] == pat[offset]) ++offset;
		else return NULL;
	}
	path += offset;

	buf = calloc(
		strlen(sub) + strlen(path) + 1,
		sizeof(char)
	);
	if (!buf) return NULL;

	strcat(buf, sub);
	strcat(buf, path);


	return buf;
}
