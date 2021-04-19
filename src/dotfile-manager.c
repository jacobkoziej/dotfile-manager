/*
 * dotfile-manager
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

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	/* initial options */
	config_t options = {
		.file_cnt = 0,
		.files    = NULL,
		.mode     = '\0'
	};


	/* parse command-line arguments */
	while (true) {
		int opt, long_index;

		char *flags = "s";
		struct option long_flags[] = {
			{"stow", no_argument, NULL, 's'},
			{0, 0, 0, 0}
		};

		opt = getopt_long(argc, argv, flags, long_flags, &long_index);

		if (opt == -1) break;  // no more flags

		switch (opt) {
			// stow
			case 's':
				options.mode = opt;
				break;

			// invalid option
			case '?':
			default:
				return EXIT_FAILURE;
		}
	}


	return EXIT_SUCCESS;
}
