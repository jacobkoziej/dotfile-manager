# DOTS

> A simple tool for managing dotfiles with VCS

Dotfile management is currently a mess. It seems that no tools follow the UNIX
philosophy when it comes to this simple task. `dots` makes managing dotfiles
extremely easy by effortlessly stowing your dotfiles into a predefined directory
(by default `~/.dotfiles`) and creating relative symbolic links to the stowed
files. The user is then free to manage the dotfile directory in the way that
best fits their workflow.

## Usage

* `dots stow ~/.vimrc` - stow `.vimrc` in the dotfile directory
* `dots unstow ~/.vimrc` - unstow `.vimrc` from the dotfile directory
* `dots deploy` - create links to all the files in the dotfile directory
* `dots help` - show program usage
* `dots info` - show program information

## Installation

**DISCLAIMER:** this project is still in development and **SUBJECT TO CHANGE**

### From Source

```bash
# Run in the project root
make install
```

## License

This project is licensed under the GPLv3. More information can be found in
[LICENSE.md](LICENSE.md)
