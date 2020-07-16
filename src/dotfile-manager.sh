#!/usr/bin/env bash
# DOTS - a simple tool for managing dotfiles with VCS
# Copyright (C) 2020  Jacob Koziej
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

STORE_DIR="${DOTFILE_STORE_DIR:-$HOME/.dotfiles}"

#
# BEGIN supplementary functions
#

fail() {
	echo "$@" >&2
	exit 1
}

getFilePath() {
	[[ "${1:0:1}" == "/" ]] && FILE_PATH=$1 || FILE_PATH="$PWD/$1" # get file path
	[[ -f $FILE_PATH ]] || fail "Error: '$1' does not exist or is not a regular file"
	FILE_PATH=$(realpath --no-symlinks $FILE_PATH) # get absolute file path
	[[ "${FILE_PATH##$HOME}" != "$FILE_PATH" ]] || fail "Error: '$1' is not in the user home directory"
}

#
# END supplementary functions
#
