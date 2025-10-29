<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: LGPL-3.0-or-later
-->
Input
=====
This page documents parts of the api related to obtaining input.

Functions
---------

---
#### `god::input::file`
From a file path, return a list of strings.
| name     | type                 |
|----------|----------------------|
| **path** | `const std::string&` |
```cpp
std::optional<std::vector<std::string>> god::input::file(const std::string& path);
```
**Returns**: [`std::optional<std::vector<std::string>>`] - A list of strings, one for each line of the file.

---
#### `god::input::stream`
From an input stream, return a list of strings.
| name     | type                 |
|----------|----------------------|
| **in**   | `std::istream&`      |
```cpp
std::vector<std::string> god::input::stream(std::istream& in);
```
**Returns**: [`std::vector<std::string>`] - A list of strings, one for each line of the input.
~~~admonish example
Read lines from standard input
```cpp
auto lines = god::input::stream(std::cin);
```
~~~