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

#define _GNU_SOURCE

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"


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

	char *out_path = malloc(strlen(path + offset) + strlen(sub) + 1);
	if (!out_path) return NULL;

	strcat(out_path, sub);
	strcat(out_path, path + offset);

	return out_path;
}
