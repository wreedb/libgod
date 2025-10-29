// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <variant>

// In parser:
// Value parseNumber(const std::string& lexeme) {
//     if (lexeme.find('.') != std::string::npos || 
//         lexeme.find('e') != std::string::npos ||
//         lexeme.find('E') != std::string::npos) {
//         // It's a float
//         double d = std::stod(lexeme);
//         if (d > god::doubleMax || d < god::doubleMin) {
//             // error: out of range
//         }
//         return d;
//     } else {
//         // It's an integer
//         std::int64_t i = std::stoll(lexeme);
//         if (i > god::integerMax || i < god::integerMin) {
//             // error: out of range (though this shouldn't happen with int64)
//         }
//         return i;
//     }
// }