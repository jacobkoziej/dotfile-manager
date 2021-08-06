/*
 * ansi-esc.h -- ANSI escape sequences
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

#ifndef DOTFILE_MANAGER_ANSI_ESC
#define DOTFILE_MANAGER_ANSI_ESC


#define ANSI_CSI "\033["


/* Select Graphic Rendition */
#define ANSI_SGR "m"

#define ANSI_SGR_RESET     "0"
#define ANSI_SGR_BOLD      "1"
#define ANSI_SGR_LIGHT     "2"
#define ANSI_SGR_ITALIC    "3"
#define ANSI_SGR_UNDERLINE "4"

#define ANSI_SGR_FOREGROUND        "3"
#define ANSI_SGR_BACKGROUND        "4"
#define ANSI_SGR_BRIGHT_FOREGROUND "9"
#define ANSI_SGR_BRIGHT_BACKGROUND "10"

#define ANSI_SGR_BLACK   "0"
#define ANSI_SGR_RED     "1"
#define ANSI_SGR_GREEN   "2"
#define ANSI_SGR_YELLOW  "3"
#define ANSI_SGR_BLUE    "4"
#define ANSI_SGR_MAGENTA "5"
#define ANSI_SGR_CYAN    "6"
#define ANSI_SGR_WHITE   "7"


#endif /* DOTFILE_MANAGER_ANSI_ESC */
