/*
 * core.c -- core functionality
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

#include "core.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "macros.h"
#include "path.h"
#include "settings.h"


static bool *keep_going = &settings.flag.keep_going;


/*
 * Free a dots_t type.
 */
void free_dots_t(dots_t **d)
{
	if (!d)  return;
	if (!*d) return;

	for (size_t i = 0; i < (*d)->n; i++) {
		target_t *t = (*d)->targets + i;
		FREE(t->src_path);
		FREE(t->dst_path);
		FREE(t->link_path);
	}

	FREE((*d)->targets);
	FREE(*d);
}

/*
 * Initialize a dots_t type.
 */
dots_t *load_targets(int argc, char **argv)
{
	assert(argc > 0);
	assert(argv);


	if (argc <= 0 || !argv) {
		errno = EINVAL;
		return NULL;
	}

	for (int i = 0; i < argc; i++) {
		if (!argv[i]) {
			errno = EINVAL;
			return NULL;
		}

		if (*argv[i] == '\0') {
			errno = ENOENT;
			return NULL;
		}
	}


	dots_t   *d;
	target_t *t;
	struct stat st;

	char *wd = settings.work_dir;
	char *sd = settings.store_dir;


	d = calloc(1, sizeof(dots_t));
	if (!d) return NULL;

	d->n = argc;
	d->targets = calloc(d->n, sizeof(target_t));
	if (!d->targets) goto error;


	for (size_t i = 0; i < d->n; i++) {
		t = d->targets + i;

		// since we're dealing with static memory there's no need to
		// allocate the input string onto the heap
		t->in_path = argv[i];

		// get absolute path
		t->src_path = path_abs(wd, t->in_path);
		if (!t->src_path) {
			if (!*keep_going) goto error;
			t->err = true;
			continue;
		}

		// get file type
		if (lstat(t->src_path, &st) < 0) {
			if (!*keep_going) goto error;
			t->err = true;
			continue;
		}
		t->type = st.st_mode;

		// get link path
		t->dst_path = path_sub(t->src_path, wd, sd);
		if (!t->dst_path) {
			if (!*keep_going) goto error;
			t->err = true;
			continue;
		}

		// get relative path for link
		t->link_path = path_rel(t->src_path, t->dst_path);
		if (!t->link_path) {
			if (!*keep_going) goto error;
			t->err = true;
			continue;
		}
	}


	return d;

error:
	free_dots_t(&d);
	return NULL;
}
