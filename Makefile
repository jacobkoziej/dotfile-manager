DESTDIR ?=
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin

all:
	@echo "Dotfile Manager is a shell script, there is nothing to do. Run \"make install\" instead."

install:
	install -v -d "$(DESTDIR)$(BINDIR)/" && install -m 0755 -v src/dotfile-manager.sh "$(DESTDIR)$(BINDIR)/dots"

uninstall:
	rm -f /usr/bin/dots
