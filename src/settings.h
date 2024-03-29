/*
 * settings.h -- control runtime behavior
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

#ifndef DOTFILE_MANAGER_SETTINGS
#define DOTFILE_MANAGER_SETTINGS


#include <stdbool.h>


typedef struct setting_flag_s {
	bool ansi_sgr_stdout;
	bool ansi_sgr_stderr;
	bool dry_run;
	bool keep_going;
} setting_flag_t;

typedef struct setting_s {
	int targets;
	setting_flag_t flag;
	char *store_dir;
	char *work_dir;
} setting_t;


extern setting_t settings;


int  setting_auto(void);
void setting_free(void);
int  setting_getopt(int argc, char **argv);


#endif /* DOTFILE_MANAGER_SETTINGS */
