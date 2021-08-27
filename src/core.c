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

#include "macros.h"

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
