# meson.build
# Copyright (C) 2021  Jacob Koziej <jacobkoziej@gmail.com>
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

dotfile_manager_sources = files(
        'core.c',
        'dotfile-manager.c',
        'path.c',
        'settings.c',
)

dotfile_manager_inc = include_directories('.')


dotfile_manager_conf_data = configuration_data()

dotfile_manager_conf_data.set(
        'program-name',
        get_option('program-name'),
)

dotfile_manager_conf_data.set(
        'program-version',
        meson.project_version(),
)

dotfile_manager_conf_data.set(
        'store-dir',
        get_option('store-dir'),
)


configure_file(
        input         : 'config.h.in',
        output        : 'config.h',
        configuration : dotfile_manager_conf_data,
)
