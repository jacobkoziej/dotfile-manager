/*
 * config.c -- configuration
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

#include "config.h"
#include "path.h"

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/* initialize configuration structure */
config_t *config_init(void)
{
	config_t *tmp = malloc(sizeof(config_t));
	if (!tmp) return NULL;

	tmp->path_cnt = 0;
	tmp->paths    = NULL;
	tmp->mode     = '\0';
	tmp->base_dir = NULL;
	tmp->stow_dir = NULL;

	return tmp;
}

/* load command-line arguments */
int config_getopt(config_t *in, int argc, char **argv)
{
	assert(in   != NULL);
	assert(argv != NULL);
	assert(in->path_cnt == 0);
	assert(in->paths    == NULL);
	assert(in->mode     == '\0');
	assert(in->base_dir == NULL);
	assert(in->stow_dir == NULL);


	/* available flags */
	char *flags = "s";
	struct option long_flags[] = {
		{"stow", no_argument, NULL, 's'},
		{0, 0, 0, 0}
	};


	/* cleanup sanity */
	int path_init_cnt = 0;


	/* parse args */
	while (true) {
		int opt, long_index;

		opt = getopt_long(argc, argv, flags, long_flags, &long_index);

		if (opt == -1) break;  // no more flags

		switch (opt) {
			// stow
			case 's':
				in->mode = opt;
				break;

			// invalid option
			case '?':
			default:
				return 0;
		}
	}


	/* parse path arguments */
	if (optind != argc) {
		in->path_cnt = argc - optind;
		in->paths = malloc(sizeof(path_t) * in->path_cnt);
		if (!in->paths) goto error;
	}

	for (int i = 0; i < in->path_cnt; i++) {
		if (path_init(argv[optind], &in->paths[i])) {
			++path_init_cnt;
			++optind;
		} else goto error;
	}

	return 1;

error:
	// path_init_cnt always zero if in->paths == NULL
	for (int i = 0; i < path_init_cnt; i++) {
		path_del(&in->paths[i]);
	}

	if (in->paths)    free(in->paths);
	if (in->base_dir) free(in->base_dir);
	if (in->stow_dir) free(in->stow_dir);

	in->path_cnt = 0;
	in->paths    = NULL;
	in->mode     = '\0';
	in->base_dir = NULL;
	in->stow_dir = NULL;

	return 0;
}

/* generate paths */
int config_paths(config_t *in)
{
	int i = 0;
	while (i < in->path_cnt) {
		if (in->paths[i].proccess) {
			in->paths[i].absolute = path_abs(in->paths[i].input);
			if (!in->paths[i].absolute) goto error;

			in->paths[i].link = path_sub(
				in->paths[i].absolute,
				in->base_dir,
				in->stow_dir
			);
			if (!in->paths[i].link) goto error;

			in->paths[i].relative = path_rel(
				in->paths[i].link,
				in->paths[i].absolute
			);
			if (!in->paths[i].relative) goto error;
		}
		++i;
	}

	return 1;

error:
	while (i >= 0) {
		if (in->paths[i].absolute) {
			free(in->paths[i].absolute);
			in->paths[i].absolute = NULL;
		}

		if (in->paths[i].link) {
			free(in->paths[i].link);
			in->paths[i].link = NULL;
		}

		if (in->paths[i].relative) {
			free(in->paths[i].relative);
			in->paths[i].relative = NULL;
		}

		--i;
	}

	return 0;
}

/* free configuration structure */
void config_free(config_t *in)
{
	if(!in) return;

	if (in->paths) {
		for (int i = 0; i < in->path_cnt; i++) {
			path_del(&in->paths[i]);
		}
		free(in->paths);
	}

	if (in->base_dir) free(in->base_dir);
	if (in->stow_dir) free(in->stow_dir);

	free(in);
}

/* load any non-overwritten defaults */
int config_set_defaults(config_t *in)
{
	char *tmp_base = NULL;
	char *tmp_stow = NULL;

	if (!in->base_dir) {
		char *home_env = getenv("HOME");
		if (!home_env) goto error;

		tmp_base = strdup(home_env);
		if (!tmp_base) goto error;
		in->base_dir = tmp_base;
	}

	if (!in->stow_dir) {
		size_t path_len = (
			strlen(in->base_dir) +
			strlen(DEFAULT_STOW_DIR) + 2
			// +2 for '/' and '\0'
		);
		char path[path_len];
		memset(path, '\0', path_len);

		strcat(path, in->base_dir);
		strcat(path, "/");
		strcat(path, DEFAULT_STOW_DIR);

		tmp_stow = strdup(path);
		if (!tmp_stow) goto error;
		in->stow_dir = tmp_stow;
	}

	return 1;

error:
	if (tmp_base) {
		free(tmp_base);
		in->base_dir = NULL;
	}

	if (tmp_stow) {
		free(tmp_stow);
		in->stow_dir = NULL;
	}

	return 0;
}
