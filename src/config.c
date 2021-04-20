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


/* load command-line arguments */
int config_getopt(config_t *in, int argc, char **argv)
{
	assert(in   != NULL);
	assert(argv != NULL);
	assert(in->path_cnt == 0);
	assert(in->paths    == NULL);


	/* available flags */
	char *flags = "s";
	struct option long_flags[] = {
		{"stow", no_argument, NULL, 's'},
		{0, 0, 0, 0}
	};


	/* cleanup sanity */
	config_t temp = *in;
	int path_init_cnt = 0;


	/* parse args */
	while (true) {
		int opt, long_index;

		opt = getopt_long(argc, argv, flags, long_flags, &long_index);

		if (opt == -1) break;  // no more flags

		switch (opt) {
			// stow
			case 's':
				temp.mode = opt;
				break;

			// invalid option
			case '?':
			default:
				return 0;
		}
	}


	/* parse path arguments */
	if (optind != argc) {
		temp.path_cnt = argc - optind;
		temp.paths = malloc(sizeof(path_t) * temp.path_cnt);
		if (!temp.paths) goto error;
	}

	for (int i = 0; i < temp.path_cnt; i++) {
		if (path_init(argv[optind], &temp.paths[i])) {
			++path_init_cnt;
			++optind;
		} else goto error;
	}


	*in = temp;
	return 1;


error:
	for (int i = 0; i < path_init_cnt; i++) {
		path_del(&temp.paths[i]);
	}
	free(temp.paths);

	return 0;
}

/* free members of config structure */
void config_free(config_t *in)
{
	if(!in) return;

	for (int i = 0; i < in->path_cnt; i++) {
		path_del(&in->paths[i]);
	}
	free(in->paths);
}
