#!/usr/bin/env python

from platform import sys;
from platform import os;
import platform;
from pathlib import Path as path;
import semver;
import shutil as sh;

project_name: str = "libgod"


def eprint(message: str):
    print(message, file=sys.stderr)

def die(message: str):
    eprint(message)
    sys.exit(1)

def verify_python():
    version: str = platform.python_version().split('.')

    major: int = int(version[0])
    minor: int = int(version[1])
    patch: int = int(version[2])

    if ((major < 3) or ((major >= 3) and (minor < 14))):
        eprint("python version >= 3.14 required (for pathlib 'Path.copy_into()')")
        eprint("either the system you are using is just old, or the administrators of it potentially still")
        eprint("python version 2 as the 'python' executable; in which case, tell them to stop doing that!")
        sys.exit(1)

def project_version() -> dict[str, str]:
    version_file: path = path(".version")
    if (version_file.exists()):
        return semver.parse(version_file.read_text().strip())
    else:
        eprint("the version file '.version' does not exist!")
        sys.exit(1)

release_files: list[path] = [
    path("doc"),
    path("src"),
    path("include"),
    path("misc"),
    path("test"),
    path("example"),
    path("LICENSES"),
    path("justfile"),
    path(".editorconfig"),
    path(".version"),
    path(".gitignore"),
    path("cliff.toml"),
    path("flake.lock"),
    path("flake.nix"),
    path("CONTRIBUTING.md"),
    path("LICENSE.md"),
    path("README.md"),
    path("meson.build"),
    path("meson.options"),
    path("REUSE.toml")
];

remove_patterns: list[str] = [
    ".build",
    "version.hpp"
];

def docopy(files: list[path]) -> path:
    vers: dict[str, str] = project_version()
    new_directory: path = path(f"{project_name}-{vers['major']}.{vers['minor']}.{vers['patch']}")
    new_directory.mkdir(parents=True, exist_ok=True)
    if not new_directory.exists():
        die("the dist directory wasn't properly created!")

    for f in files:
        if f.exists():
            f.copy_into(new_directory)
        else:
            die(f"could not copy {f.as_posix()} into {new_directory.as_posix()}")

    return new_directory
        
def prune(patterns: list[str], dir: path):
    for pat in patterns:
        for found in dir.rglob(pat):
            if found.is_dir():
                sh.rmtree(found)
                print(f"pruned directory: {found.relative_to(dir).as_posix()} from {dir.as_posix()}")
            else:
                found.unlink()
                print(f"pruned file: {found.relative_to(dir).as_posix()} from {dir.as_posix()}")

verify_python()
new_dir: path = docopy(release_files)
prune(remove_patterns, new_dir)