@default:
    @meson setup .build --wipe
    @meson compile -C .build
    LD_LIBRARY_PATH=$PWD/.build .build/god_token sample.god
