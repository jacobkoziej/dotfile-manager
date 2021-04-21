/*
 * path.c -- file path manipulation
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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


/* get a relative path from an absolute path */
char *path_rel(char *to, char *from)
{
	assert(to   != NULL);
	assert(from != NULL);

	/* remove common prefix */
	while (*to == *from) {
		++to; ++from;
	}

	// strip only up to dir
	while (*to != '/') {
		--to; --from;
	}

	++to; ++from;  // strip '/'


	/* generate relative path */
	int    dir_cnt  = 0;
	size_t from_siz = strlen(from);

	for (size_t i = 0; i < from_siz; i++) {
		if (from[i] == '/') ++dir_cnt;
	}

	char *output = calloc(
		strlen(to) + (strlen("../") * dir_cnt) + 1,
		sizeof(char)
	);
	if (!output) return NULL;

	for (int i = 0; i < dir_cnt; i++) {
		strcat(output, "../");
	}
	strcat(output, to);

	return output;
}

/* perform a path substitution */
char *path_sub(char *path, char *patt, char *sub)
{
	assert(path != NULL);
	assert(patt != NULL);
	assert(sub  != NULL);

	size_t offset   = 0;
	size_t patt_len = strlen(patt);

	while (offset < patt_len) {
		if (path[offset] == patt[offset]) {
			++offset;
		} else {
			return NULL;
		}
	}

	char *out_path = calloc(
		strlen(path + offset) + strlen(sub) + 1,
		sizeof(char)
	);
	if (!out_path) return NULL;

	strcat(out_path, sub);
	strcat(out_path, path + offset);

	return out_path;
}

/* check if a folder exists */
int path_dir_check(char *path)
{
	struct stat info;

	// doesn't exist
	if (lstat(path, &info)) return 0;

	// check if it's a directory
	if (S_ISDIR(info.st_mode)) return 1;
	else return -1;
}

/* initialize a path structure */
int path_init(char *path, path_t *in)
{
	assert(path != NULL);
	assert(in   != NULL);

	path_t temp = {
		.proccess  = false,
		.type      = '\0',
		.input     = NULL,
		.absolute  = NULL,
		.relative  = NULL,
	};

	struct stat info;
	if (!lstat(path, &info)) {
		if (S_ISREG(info.st_mode)) {
			temp.proccess = true;
			temp.type = 'f';
		} else if (S_ISDIR(info.st_mode)) {
			temp.proccess = true;
			temp.type = 'd';
		} else if (S_ISLNK(info.st_mode)) {
			temp.type = 'l';
		}
	}

	temp.input = strdup(path);
	if (!temp.input) return 0;

	*in = temp;
	return 1;
}

/* make parent directories as needed */
int path_mkdir(char *path, mode_t mode)
{
	assert (path != NULL);

	switch (path_dir_check(path)) {
		case 0: break;
		case 1: return 1;

		case -1:
		default: return 0;
	}

	struct stat info;

	char *path_cp = strdup(path);
	if (!path_cp) goto error;

	for (char *c = strchr(path_cp + 1, '/'); c; c = strchr(c + 1, '/')) {
		*c = '\0';

		// check if dir doesn't exist or valid existing dir
		if (lstat(path_cp, &info)) {
			if (mkdir(path_cp, mode)) goto error;
		} else if (!S_ISDIR(info.st_mode)) goto error;

		*c = '/';
	}
	if (lstat(path_cp, &info) && mkdir(path_cp, mode)) goto error;

	free(path_cp);
	return 1;

error:
	free(path_cp);
	return 0;
}

/* free members of path structure */
void path_del(path_t *in)
{
	if (!in) return;

	free(in->input);
	free(in->absolute);
	free(in->link);
	free(in->relative);
}
