/*
 * path.h -- path tools
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

#ifndef DOTFILE_MANAGER_PATH
#define DOTFILE_MANAGER_PATH


#include <sys/stat.h>


int   mkdirp(const char *path, mode_t mode);
char *path_abs(char *wd, char *path);
char *path_cat(char *org, char *add);
char *path_rel(char *src, char *dst);
char *path_sub(char *path, char *pat, char *sub);


#endif /* DOTFILE_MANAGER_PATH */
