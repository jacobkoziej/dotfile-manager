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


dots_t dots = {
	.targets = NULL,
	.n       = 0,
};

static bool *keep_going = &settings.flag.keep_going;


/*
 * Free a dots_t type.
 */
void free_dots_t(void)
{
	for (size_t i = 0; i < dots.n; i++) {
		target_t *t = dots.targets + i;
		FREE(t->src_path);
		FREE(t->dst_path);
		FREE(t->link_path);
	}

	FREE(dots.targets);
}

/*
 * Initialize a dots_t type.
 */
int load_targets(int argc, char **argv)
{
	assert(argc > 0);
	assert(argv);


	if (argc <= 0 || !argv) {
		errno = EINVAL;
		return -1;
	}

	for (int i = 0; i < argc; i++) {
		if (!argv[i]) {
			errno = EINVAL;
			return -1;
		}

		if (*argv[i] == '\0') {
			errno = ENOENT;
			return -1;
		}
	}


	target_t *t;
	struct stat st;

	char *wd = settings.work_dir;
	char *sd = settings.store_dir;


	dots.n = argc;
	dots.targets = calloc(dots.n, sizeof(target_t));
	if (!dots.targets) goto error;


	for (size_t i = 0; i < dots.n; i++) {
		t = dots.targets + i;

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


	return 0;

error:
	free_dots_t();
	return -1;
}
