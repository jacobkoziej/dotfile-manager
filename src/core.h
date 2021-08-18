/*
 * core.h -- core functionality
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

#ifndef DOTFILE_MANAGER_CORE
#define DOTFILE_MANAGER_CORE


#include <stddef.h>
#include <sys/stat.h>


typedef struct target_s {
	mode_t *type;

	char *in_path;
	char *abs_path;
	char *link_path;
} target_t;

typedef struct dots_s {
	target_t *targets;
	size_t    n;
} dots_t;


#endif /* DOTFILE_MANAGER_CORE */
