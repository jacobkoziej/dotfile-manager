/*
 * path.c -- file path manipulation
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

#ifndef PATH_H
#define PATH_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/stat.h>


#define path_abs(path) canonicalize_file_name(path)


char *path_rel(char *to, char *from);
char *path_sub(char *path, char *patt, char *sub);
int   path_mkdir(char *path, mode_t mode);

#endif /* PATH_H */
