/*
 * config.h -- configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include "path.h"

#include <stdbool.h>


# define DEFAULT_STOW_DIR ".dotfiles"


typedef struct config_s {
	struct {
		bool dry_run : 1;
	} flags;
	int     path_cnt;
	path_t *paths;
	char    mode;
	char   *base_dir;
	char   *stow_dir;
} config_t;


config_t *config_init(void);
int       config_getopt(config_t *in, int argc, char **argv);
int       config_paths(config_t *in);
int       config_set_defaults(config_t *in);
void      config_free(config_t *in);

#endif /* CONFIG_H */
