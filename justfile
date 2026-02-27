
project := "libgod"
version := `cat .version`

builddir := quote(justfile_directory() / ".build")
cachedir := quote(justfile_directory() / ".cache")
docsdir := quote(justfile_directory() / "doc/.build")
doxyfile := quote(justfile_directory() / "doc/doxyfile")

default:
    @just --list

build:
    meson setup {{builddir}} --prefix=/usr --buildtype=debugoptimized
    meson compile -C {{builddir}}

test:
    meson test -C {{builddir}}

[group("release")]
dist:
    misc/scripts/make-dist.py

commit:
    convco commit -i

bump type="patch":
    @pysemver bump {{type}} {{version}}

new-test name:
    cp misc/templates/test.cpp test/{{name}}.test.cpp
    cp misc/templates/test.god test/aux/{{name}}.god
    @echo dont forget to edit test/meson.build to add it

[group("maintainence")]
reuse:
    @reuse lint

[group("build")]
clean-build:
    -rm -rf {{builddir}}

[group("docs")]
doxygen:
    doxygen {{doxyfile}}

[group("docs")]
clean-docs:
    -rm -rf {{docsdir}}

[group("maintainence")]
clean-cache:
    -rm -rf {{cachedir}}

[group("nix")]
flake action="build":
    {{ if action == 'build' { `nix build` } else if action == 'update' { `nix flake update` } else if action == 'clean' { `rm -f result` } else { `echo` } }}

[group("nix")]
flake-update:
    nix flake update

[group("nix")]
flake-build:
    nix build

[group("nix")]
flake-clean:
    -rm -f result

[group("maintainence")]
clean: clean-build clean-cache flake-clean clean-docs

noop:
    @echo