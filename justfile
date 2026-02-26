
project := "libgod"
version := `cat .version`

default:
    @just --list

dist:
    mkdir -p {{project}}-{{version}}/doc
    cp -t {{project}}-{{version}} -a \
        *.md *.toml .clang* .version \
        .editorconfig justfile test/ \
        misc/ src/ include/ example/ \
        doc/doxyfile

commit:
    convco commit -i

bump type="patch":
    @pysemver bump {{type}} {{version}}

new-test name:
    cp misc/templates/test.cpp test/{{name}}.test.cpp
    cp misc/templates/test.god test/aux/{{name}}.god
    @echo dont forget to edit test/meson.build to add it