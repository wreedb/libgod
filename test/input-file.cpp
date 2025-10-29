// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <god.hpp>
#include <optional>
#include "god-test.hpp"

int main() {
    std::string fp = SAMPLE_FILE;
    std::optional<std::vector<std::string>> infile = god::input::file(fp);
    if (!infile)
        return 1;
    std::vector<std::string> lines = infile.value();
    if (lines.size() == 0)
        return 1;
    return 0;
}