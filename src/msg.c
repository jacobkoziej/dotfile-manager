/*
 * msg.c -- program messages
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

#include "msg.h"

#include <stdarg.h>
#include <stdio.h>


/* print a warning message */
int msg_warn(char *format, ...)
{
	va_list args;
	va_start(args, format);

	fprintf(stderr, "warning: ");
	vfprintf(stderr, format, args);

	va_end(args);
	return 1;
}
