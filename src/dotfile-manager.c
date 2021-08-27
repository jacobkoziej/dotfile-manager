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

#include <stdlib.h>

#include "core.h"
#include "settings.h"


static dots_t *dots = NULL;


void cleanup(void)
{
	setting_free();
	free_dots_t(&dots);
}

int main(int argc, char **argv)
{
	static int optind, targets;


	atexit(cleanup);


	if (setting_auto() < 0) return EXIT_FAILURE;
	if ((optind = setting_getopt(argc, argv)) < 0) return EXIT_FAILURE;


	if ((targets = argc - optind) <= 0) return EXIT_FAILURE;

	dots = load_targets(targets, argv + optind);
	if (!dots) return EXIT_FAILURE;


	return EXIT_SUCCESS;
}
