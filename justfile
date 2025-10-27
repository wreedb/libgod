@default:
    @meson setup .build -D c_args='' -D cpp_args='' --buildtype=plain --wipe
    @meson compile -C .build
    @meson test -C .build

@docs:
    @mkdocs serve
