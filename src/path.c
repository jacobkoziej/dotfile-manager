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
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "macros.h"


/*
 * Create directories and parents directories as needed (no error if existing).
 */
int mkdirp(const char *path, mode_t mode)
{
	assert(path);


	if (*path == '\0') return -1;

	char *path_dup = strdup(path);
	if (!path_dup) return -1;

	struct stat st;


	// generate all parents as necessary
	for (char *d = strchr(path_dup + 1, '/'); d; d = strchr(d + 1, '/')) {
		*d = '\0';
		if (mkdir(path_dup, mode) < 0 && errno != EEXIST) goto error;
		*d = '/';
	}
	if (mkdir(path_dup, mode) < 0 && errno != EEXIST) goto error;

	// make sure the last element is a directory
	if (lstat(path_dup, &st) || !S_ISDIR(st.st_mode)) goto error;


	FREE(path_dup);
	return 0;

error:
	FREE(path_dup);
	return -1;
}

/*
 * Return the absolute file path.
 */
char *path_abs(char *wd, char *path)
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

	if (!path) {
		errno = EINVAL;
		return NULL;
	}

	if (*path == '\0') {
		errno = ENOENT;
		return NULL;
	}

	if (wd && *wd != '/') {
		errno = EINVAL;
		return NULL;
	}


	char *buf, *cwd = NULL;
	char *buf_lim, *nul, *head, *tail;
	size_t buf_siz = PATH_MAX + 1;
	bool rel_path = *path != '/';


	if (rel_path && !wd) {
		cwd = get_current_dir_name();
		if (!cwd) return NULL;
	}
	if (rel_path) buf_siz += strlen((wd) ? wd : cwd);

	buf = calloc(buf_siz, sizeof(char));
	if (!buf) goto error;

	if (rel_path) strcpy(buf, (wd) ? wd : cwd);
	else *buf = '/';  // we need root!
	nul = buf + buf_siz - PATH_MAX - rel_path;
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
			if (nul > buf + 1) {
				while ((--nul)[-1] != '/');
				*nul = '\0';
			}
			continue;
		}

		// make sure we have a trailing '/'
		// and prevent an invalid read of size 1
		if (nul - 1 < buf || nul[-1] != '/') *nul++ = '/';

		// resize if necessary
		if (nul + diff >= buf_lim) {
			uintptr_t nul_offset = nul - buf;
			char *buf_new = NULL;

			buf_siz += (diff + 1 > PATH_MAX) ? diff + 1 : PATH_MAX;

			buf_new = realloc(buf, buf_siz);
			if (!buf_new) goto error;

			buf = buf_new;
			buf_lim = buf + buf_siz;
			nul = buf + nul_offset;
		}

		nul = mempcpy(nul, head, diff);
		*nul = '\0';
	}

	// (if it exists) remove the trailing '/'
	if (nul - 1 > buf && nul[-1] == '/') *--nul = '\0';

	// shrink the buffer to fit the generated path
	char *out = realloc(buf, sizeof(char) * (nul - buf + 1));
	if (!out) goto error;


	FREE(cwd);
	return out;

error:
	FREE(buf);
	FREE(cwd);
	return NULL;
}

/*
 * Concatenate existing paths.
 */
char *path_cat(char *org, char *add)
{
	if (!add) {
		errno = EINVAL;
		return NULL;
	}

	// if we have no original string,
	// or if we get an absolute path,
	// we need to guarantee a string on the heap
	if (!org || *add == '/') return strdup(add);


	char   *buf;
	size_t  buf_siz = 0;
	size_t  org_len = strlen(org);


	if (org[org_len - 1] != '/') ++buf_siz;  // ensue trailing '/'
	buf_siz += org_len + strlen(add) + 1;

	buf = calloc(buf_siz, sizeof(char));
	if (!buf) return NULL;

	strcat(buf, org);
	if (org[org_len - 1] != '/') strcat(buf, "/");
	strcat(buf, add);


	return buf;
}

/*
 * Generate a relative path from an absolute source to an absolute destination.
 */
char *path_rel(char *src, char *dst)
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
char *path_sub(char *path, char *pat, char *sub)
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
