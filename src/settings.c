/*
 * settings.c -- control runtime behavior
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

#include "settings.h"
#include "settings_private.h"

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "macros.h"
#include "path.h"


setting_t settings = {
	.flag = {
		.ansi_sgr_stdout = false,
		.ansi_sgr_stderr = false,
		.dry_run         = false,
		.keep_going      = false,
	},
	.store_dir = NULL,
	.work_dir  = NULL,
};

static setting_flag_t *flag = &settings.flag;

static char *flags = ":kns:w:";
static struct option long_flags[] = {
	{     "color", optional_argument, NULL,   0},
	{   "dry-run",       no_argument, NULL, 'n'},
	{"keep-going",       no_argument, NULL, 'k'},
	{ "store-dir", required_argument, NULL, 's'},
	{  "work-dir", required_argument, NULL, 'w'},
	{           0,                 0,    0,   0},
};


/*
 * Set options automatically.
 */
int setting_auto(void)
{
	if (ansi_sgr_mode(NULL) < 0) goto error;

	if (set_store_dir(NULL, NULL) < 0) goto error;


	return 0;

error:
	return -1;
}

/*
 * Free dynamic members of the global settings structure.
 */
void setting_free(void)
{
	FREE(settings.store_dir);
}

/*
 * Parse command-line options.
 */
int setting_getopt(int argc, char **argv)
{
	assert(argv);


	opterr = 0;  // disable getopt() error messages

	while (true) {
		int opt, long_index;
		const char *name;

		opt = getopt_long(argc, argv, flags, long_flags, &long_index);
		if (opt == -1) break;  // nothing left to parse

		switch (opt) {
			// keep-going
			case 'k':
				flag->keep_going = true;
				break;

			// dry-run
			case 'n':
				flag->dry_run = true;
				break;

			// store-dir
			case 's':
				FREE(settings.store_dir);
				if (set_store_dir(settings.work_dir, optarg) < 0) goto error;
				break;

			// work-dir
			case 'w':
				settings.work_dir = optarg;
				break;


			// long options
			case 0:
				name = long_flags[long_index].name;
				if (parse_long_flags(name) < 0) goto error;
				break;


			case '?':
			case ':':
			default:
				// TODO: handle errors
				goto error;
		}
	}


	return optind;

error:
	return -1;
}


/*
 * Set the ANSI SGR mode for stdout and stderr.
 */
static int ansi_sgr_mode(char *mode)
{
	if (!mode || !strcmp(mode, "auto")) {
		flag->ansi_sgr_stdout = isatty(STDOUT_FILENO);
		flag->ansi_sgr_stderr = isatty(STDERR_FILENO);
	} else if (!strcmp(mode, "full")) {
		flag->ansi_sgr_stdout = true;
		flag->ansi_sgr_stderr = true;
	} else if (!strcmp(mode, "none")) {
		flag->ansi_sgr_stdout = false;
		flag->ansi_sgr_stderr = false;
	} else if (!strcmp(mode, "stderr")) {
		flag->ansi_sgr_stdout = false;
		flag->ansi_sgr_stderr = true;
	} else if (!strcmp(mode, "stdout")) {
		flag->ansi_sgr_stdout = true;
		flag->ansi_sgr_stderr = false;
	} else {
		return -1;
	}


	return 0;
}

/*
 * Parse long options.
 */
static int parse_long_flags(const char *name)
{
	if (!strcmp(name, "color") && ansi_sgr_mode(optarg) < 0) goto error;


	return 0;

error:
	return -1;
}

/*
 * Set the store directory.
 */
static int set_store_dir(char *wd, char *dir)
{
	char *buf, *tmp;


	tmp = (wd) ? wd : getenv("HOME");
	if (!tmp) return -1;

	buf = path_cat(tmp, (dir) ? dir : DEFAULT_STORE_DIR);
	if (!buf) return -1;

	settings.store_dir = buf;


	return 0;

}
