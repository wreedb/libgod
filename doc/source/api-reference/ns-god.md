<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: LGPL-3.0-or-later
-->
Namespace god
=============
This is the top-level namespace of libgod.

Functions
---------

---
#### `god::incr`
Increments an integer by one.
| name | type       |
|------|------------|
| n    | const int& |
```cpp
int god::incr(const int& n);
```
**Returns**: [`int`] - the integer plus 1.

---
#### `god::decr`
Decrements an integer by one.
| name | type       |
|------|------------|
| n    | const int& |
```cpp
int god::decr(const int& n);
```
**Returns**: [`int`] - the integer minus 1.

---
#### `god::add`
Adds two integers.
| name         | type       |
|--------------|------------|
| firstAddend  | const int& |
| secondAddend | const int& |
```cpp
int god::add(const int& firstAddend, const int& secondAddend);
```
**Returns**: [`int`] - the sum of the two integers

---
#### `god::subtract`
Subtracts one integer from another.
| name       | type       |
|------------|------------|
| minuend    | const int& |
| subtrahend | const int& |
```cpp
int god::subtract(const int& minuend, const int& subtrahend);
```
**Returns**: [`int`] - The difference.

---
#### `god::multiply`
Multiplies two integers.
| name         | type       |
|--------------|------------|
| multiplicand | const int& |
| muliplier    | const int& |
```cpp
int god::multiply(const int& multiplicand, const int& multiplier);
```
**Returns**: [`int`] - The product of the two integers

---
#### `god::divide`
Divides two integers, returning the quotient and remainder.
| name     | type       |
|----------|------------|
| dividend | const int& |
| divisor  | const int& |
```cpp
int god::divide(const int& dividend, const int& divisor);
```
**Returns**: [`std::pair<int, int>`] - The quotient and remainder.