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

fatal() {
	echo "$@" >&2
	exit 1
}

getFilePath() {
	local path
	[[ "${1:0:1}" == "/" ]] && path=$1 || path="$PWD/$1" # get file path
	[[ -f $path ]] || fatal "Error: '$1' does not exist or is not a regular file"
	path=$(realpath --no-symlinks $path) # get absolute file path
	[[ "${path##$HOME}" != "$path" ]] || fatal "Error: '$1' is not in the user home directory"
	echo "$path"
}

getSymLinkPath() {
	# path argument must be absolute path
	local path="$(realpath $1)"
	[[ "$path" != "$1" ]] && echo "$path"
}

#
# END supplementary functions
#
