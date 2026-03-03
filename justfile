
project := "libgod"
version := `cat .version`

builddir := quote(justfile_directory() / ".build")
cachedir := quote(justfile_directory() / ".cache")

docoutput := quote(justfile_directory() / "doc/build")
htmldir := quote(justfile_directory() / "doc/build/html")
doxyfile := quote(justfile_directory() / "doc/doxyfile")
xmldir := quote(justfile_directory() / "doc/source/doxygen/xml")
docsource := quote(justfile_directory() / "doc/source")

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
docs: doxygen
    sphinx-build {{docsource}} {{docoutput}}

[group("docs")]
doxygen:
    doxygen {{doxyfile}}

[group("docs")]
clean-docs:
    -rm -rf {{htmldir}} {{xmldir}}

[group("maintainence")]
clean-cache:
    -rm -rf {{cachedir}}

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
changelog:
    @git cliff

[group("maintainence")]
changelog-range range:
    @git cliff {{range}}

[group("maintainence")]
clean: clean-build clean-cache flake-clean clean-docs

noop:
    @echo