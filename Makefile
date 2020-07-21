all:
	@echo "Dotfile Manager is a shell script, there is nothing to do. Run \"make install\" instead."

install:
	cp -f ./src/dotfile-manager.sh /usr/bin/dots
	chmod 755 /usr/bin/dots

uninstall:
	rm -f /usr/bin/dots
