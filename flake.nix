{
    description = "Official reference implementation of the God data language";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
        flake-utils.url = "github:numtide/flake-utils";
    };

    outputs = { self, nixpkgs, flake-utils }:
        flake-utils.lib.eachDefaultSystem (system:
            let
                pkgs = nixpkgs.legacyPackages.${system};
                lib  = nixpkgs.legacyPackages.${system}.lib;
                stdenv = pkgs.llvmPackages_21.libcxxStdenv;
                version = builtins.readFile ./.version;

                nativeBuildInputs = with pkgs; [
                    meson
                    ninja
                ];

                buildInputs = [ pkgs.llvmPackages.libcxx ];

                libgod-package = stdenv.mkDerivation {
                    pname = "libgod";
                    inherit version;
                    src = ./.;

                    meta = {
                        description = "Official reference implementation of the God data language";
                        long-description = ''
                            C++ parsing library for the God data serialization format. The library is
                            exception-free while retaining highly robust error reporting information,
                            an intuitive API as well as convenient tools like outputting directly to
                            JSON.
                        '';
                        homepage = "https://wreedb.github.io/god";
                        license = lib.licenses.lgpl3Plus;
                    };

                    inherit nativeBuildInputs buildInputs;

                    mesonBuildType = "release";
                    mesonFlags = [
                        "-D strip=true"
                    ];
                };

            in {
                packages.libgod = libgod-package;
                packages.default = libgod-package;

                devShells.default = pkgs.mkShell {
                    inherit nativeBuildInputs buildInputs;
                };
            }
        );
        
}