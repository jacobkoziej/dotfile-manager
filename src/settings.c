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
#include <string.h>
#include <unistd.h>


setting_t settings = {
	.flag = {
		.ansi_sgr_stdout = false,
		.ansi_sgr_stderr = false,
		.dry_run         = false,
	},
};

static setting_flag_t *flag = &settings.flag;

static char *flags = ":n";
static struct option long_flags[] = {
	{  "color", optional_argument, NULL,   0},
	{"dry-run",       no_argument, NULL, 'n'},
	{        0,                 0,    0,   0},
};


/*
 * Set options automatically.
 */
int setting_auto(void)
{
	if (ansi_sgr_mode(NULL) < 0) goto error;


	return 0;

error:
	return -1;
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
			// dry-run
			case 'n':
				flag->dry_run = true;
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