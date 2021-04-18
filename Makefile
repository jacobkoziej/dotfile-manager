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

JOBS != nproc

MESON  ?= meson
NINJA  ?= ninja
NFLAGS ?= -j$(JOBS)

BUILD ?= build


.PHONY: all
all:
	@$(NINJA) $(NFLAGS) -C $(BUILD)

.PHONY: install
install:
	@DESTDIR=$(DESTDIR) $(NINJA) $(NFLAGS) install -C $(BUILD)

.PHONY: uninstall
uninstall:
	@DESTDIR=$(DESTDIR) $(NINJA) $(NFLAGS) uninstall -C $(BUILD)

.PHONY: configure
configure:
	@$(MESON) $(MFLAGS) $(BUILD)

.PHONY: clean
clean:
	@rm -vrf $(BUILD)
