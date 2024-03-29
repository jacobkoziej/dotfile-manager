/*
 * settings_private.h -- control runtime behavior
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

#ifndef DOTFILE_MANAGER_SETTINGS_PRIVATE
#define DOTFILE_MANAGER_SETTINGS_PRIVATE


static int ansi_sgr_mode(char *mode);
static int parse_long_flags(const char *name);
static int parse_short_flags(int val);
static int set_store_dir(char *dir);
static int set_work_dir(char *dir);


#endif /* DOTFILE_MANAGER_SETTINGS_PRIVATE */
