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

#include <stdlib.h>


int main(int argc, char **argv)
{
	/* load user options */
	config_t *options = config_init(".dotfiles");
	if (!options) return EXIT_FAILURE;
	if (!config_getopt(options, argc, argv)) goto error;
	if (!config_paths(options)) goto error;


	/* cleanup */
	config_free(options);
	options = NULL;


	return EXIT_SUCCESS;


error:
	config_free(options);
	options = NULL;

	return EXIT_FAILURE;
}
