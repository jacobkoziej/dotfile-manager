/*
 * settings.c -- control runtime behavior
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

#include "settings.h"

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>


setting_t settings = {
	.flag = {
		.ansi_sgr = false,
		.dry_run  = false,
	},
};

static setting_flag_t *flag = &settings.flag;


/*
 * Parse command-line options.
 */
int setting_getopt(int argc, char **argv)
{
	assert(argv);


	static char *flags = ":n";
	static struct option long_flags[] = {
		{"dry-run", no_argument, NULL, 'n'},
		{        0,           0,    0,   0},
	};

	opterr = 0;  // disable getopt() error messages


	while (true) {
		int opt, long_index;

		opt = getopt_long(argc, argv, flags, long_flags, &long_index);
		if (opt == -1) break;  // nothing left to parse

		switch (opt) {
			// dry-run
			case 'n':
				flag->dry_run = true;
				break;

			case '?':
			case ':':
			default:
				// TODO: handle errors
				return -1;
		}
	}


	return optind;
}
