@default:
    @meson setup .build -D c_args='' -D cpp_args='' --buildtype=plain --wipe
    @meson compile -C .build
    @meson test -C .build

[group("documentation")]
@docs-build:
    @mdbook build doc

[group("documentation")]
@docs-serve:
    @mdbook serve doc --open
