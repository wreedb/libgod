# SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
#
# SPDX-License-Identifier: LGPL-3.0-or-later

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

[group("documentation")]
@changelog:
    @git-cliff