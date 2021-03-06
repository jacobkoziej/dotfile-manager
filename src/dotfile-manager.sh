#!/usr/bin/env bash
# DOTS - a simple tool for managing dotfiles with VCS
# Copyright (C) 2020  Jacob Koziej <jacobkoziej@gmail.com>
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

set -euo pipefail

STORE_DIR="${DOTFILE_STORE_DIR:-$HOME/.dotfiles}"
[[ -d "$STORE_DIR" ]] || mkdir -p "$STORE_DIR"

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

existCheck() {
	# path argument must be processed by getFilePath first
	if [[ "${1/$STORE_DIR/}" != "$1" ]]; then
		## file is in $STOW_DIR
		[[ -f "${1/$STORE_DIR/$HOME}" ]] && echo "true" || echo "false"
	else
		## file is not in $STOW_DIR
		[[ -f "${1/$HOME/$STORE_DIR}" ]] && echo "true" || echo "false"
	fi
}

#
# END supplementary functions
#

#
# BEGIN main functions
#

stow() {
	local temp
	local file_paths=()

	while [[ "$#" != "0" ]]; do
		temp=$(getFilePath $1)
		[[ -z $(getSymLinkPath $temp) ]] || [[ "$(getSymLinkPath $temp)" =~ "$STORE_DIR" ]] || fatal "Error: '$1' is a foreign symbolic link"
		if [[ ! -d "${temp/$HOME/$STORE_DIR}" ]]; then
			local dir_create
			dir_create="${temp/$HOME/$STORE_DIR}"
			mkdir -p "${dir_create%/*}"
		fi
		file_paths+=("$temp")
		shift
	done

	local path
	for path in ${file_paths[@]}; do
		if [[ "$(existCheck $path)" == "true" ]]; then
			echo "Skipping: '$path' already stowed"
		else
			mv "$path" "${path/$HOME/$STORE_DIR}"
			echo "Moved: '$path' -> '${path/$HOME/$STORE_DIR}'"
			ln --symbolic --relative "${path/$HOME/$STORE_DIR}" "$path"
			echo "Linked: '${path/$HOME/$STORE_DIR}' -> '$path'"
		fi
	done

	return 0
}

unstow() {
	local temp
	local file_paths=()

	while [[ "$#" != "0" ]]; do
		temp=$(getFilePath $1)
		[[ -z $(getSymLinkPath $temp) ]] || [[ "$(getSymLinkPath $temp)" =~ "$STORE_DIR" ]] || fatal "Error: '$1' is a foreign symbolic link"
		if [[ "$(existCheck $temp)" == "false" ]]; then
			echo "Skipping: '$1' is not stowed"
			shift
			continue
		fi
		file_paths+=("$temp")
		shift
	done

	local path
	for path in ${file_paths[@]}; do
		[[ "$(getSymLinkPath $temp)" =~ "$STORE_DIR" ]] || path="${path/$STORE_DIR/$HOME}"
		mv  "${path/$HOME/$STORE_DIR}" "$path"
		echo "Restored: '$path'"
	done

	# delete any empty directories in $STORE_DIR
	find "${STORE_DIR}" -empty -type d -delete

	return 0
}

deploy() {
	local path
	local file_paths=()
	local conflict_paths=()

	for path in $(find $STORE_DIR -type f -print | grep -v -E '.git/|.md'); do
		if [[ "$(existCheck $path)" == "false" ]]; then
			file_paths+=("$path")
		else
			conflict_paths+=("${path/$STORE_DIR/$HOME}")
		fi
	done

	local dir_check
	if [[ -z ${conflict_paths[@]} ]]; then
		for path in ${file_paths[@]}; do
			dir_check="${path/$STORE_DIR/$HOME}"
			[[ -d "${dir_check%/*}" ]] || mkdir -p "${dir_check%/*}"
			ln --symbolic --relative "$path" "${path/$STORE_DIR/$HOME}"
			echo "Linked: '$path' -> '${path/$STORE_DIR/$HOME}'"
		done
		echo "Deployed dotfiles successfully"
	else
		for path in ${conflict_paths[@]}; do
			echo "Error: '$path' exists"
		done
		fatal "Clean home directory to deploy"
	fi

	return 0
}

info() {
	echo "DOTS - a simple tool for managing dotfiles with VCS"
	echo "Copyright (C) 2020  Jacob Koziej <jacobkoziej@gmail.com>"
	echo "License: GPLv3"
	echo "Version: 0.1.2"
}

tldr() {
	echo "dots stow <file path>"
	echo "	stow file in dotfile directory and create symlink"
	echo "dots unstow <file path>"
	echo "	restore file from dotfile directory"
	echo "dots deploy"
	echo "	create symbolic links to files in dotfile directory"
	echo "dots help"
	echo "	show usage information"
	echo "dots info"
	echo "	show program information"
}

#
# END main functions
#

[[ "$#" == "0" ]] && fatal "Error: no command, run 'dots help' for command options"

case $1 in
	stow) shift; stow "$@";;
	unstow) shift; unstow "$@";;
	deploy) shift; deploy "$@";;
	help) tldr;;
	info) info;;
	*) fatal "Error: unknown command '$1'"
esac

exit 0
