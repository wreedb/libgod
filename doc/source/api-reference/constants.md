<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: LGPL-3.0-or-later
-->
Constants
=========

#### `god::integerMax`
The ceiling (maximum) for integer values supported by **God**.
```cpp
constexpr std::int64_t god::integerMin;
```
**Value**: [`std::int64_t`] - The result of:
```cpp
std::numeric_limits<std::int64_t>::max();
```
---
#### `god::integerMin`
The floor (minimum) for integer values supported by **God**.
```cpp
constexpr std::int64_t god::integerMin;
```
**Value**: [`std::int64_t`] - The result of:
```cpp
std::numeric_limits<std::int64_t>::min();
```
---
#### `god::doubleMax`
The ceiling (maximum) for double values supported by **God**.
```cpp
constexpr double god::doubleMax;
```
**Value**: [`double`] - The result of:
```cpp
std::numeric_limits<double>::max();
```
---
#### `god::doubleMin`
The floor (minimum) for double values supported by **God**.
```cpp
constexpr double god::doubleMin;
```
**Value**: [`double`] - The result of:
```cpp
std::numeric_limits<double>::lowest();
```