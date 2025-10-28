Token
=====
This page documents parts of the api related to tokens and tokenizing.

Enums
-----
---
#### `god::tokenType`
An enumeration for all possible token types
* `leftBrace`
* `leftBracket`
* `rightBrace`
* `leftBrace`
* `semicolon`
* `equal`
* `identifier`
* `string`
* `number`
* `boolTrue`
* `boolFalse`
* `nullValue`

Structs
-------
---
#### `god::token`
This struct is used to contain all essential information about every token.
| name            | type                              | description                            |
|-----------------|-----------------------------------|----------------------------------------|
| **type**        | [`god::tokenType`](#godtokentype) | the token type                         |
| **lexeme**      | `std::string`                     | the literal lexical value of the token |
| **lineBegin**   | `int`                             | the line the token begins on           |
| **lineEnd**     | `int`                             | the line the token ends on             |
| **columnBegin** | `int`                             | the column the token begins at         |
| **columnEnd**   | `int`                             | the column the token ends at           |
```cpp
struct god::token;
```

---
#### `god::cursor`
Tracks the state of the tokenizer; line-by-line, column by column.
| name   | type                              | description                                |
|--------|-----------------------------------|--------------------------------------------|
| lines  | `const std::vector<std::string>*` | Pointer to a list containing lines of text |
| line   | `int`                             | The line number the cursor is operating on |
| column | `int`                             | The column the cursor is operating on      |
```cpp
struct god::cursor;
```

Functions
---------

---
#### `god::tokenTypeToString`
Provides a string represtation of a [`god::tokenType`](#godtokentype) for introspection purposes
| name  | type              |
|-------|-------------------|
| **t** | `god::tokenType&` |
```cpp
std::string tokenTypeToString(god::tokenType& t);
```
**Returns**: [`std::string`] - A string representation of the token type.

---
#### `god::peek`
Look ahead one column with a cursor.
| name       | type                 |
|------------|----------------------|
| **cursor** | `const god::cursor&` |
```cpp
inline char god::peek(const cursor& cursor);
```
**Returns**: [`char`] - The character in the next column.

---
#### `god::advance`
Advance one column with a cursor
| name       | type                               |
|------------|------------------------------------|
| **cursor** | [`god::cursor&`](#godcursor)       |
```cpp
inline void god::advance(cursor& cursor);
```
**Returns**: [`void`]

---
#### `god::atEnd`
Determine if we have reached the end of a cursor's input.
| name       | type                 |
|------------|----------------------|
| **cursor** | `const god::cursor&` |
```cpp
inline bool god::atEnd(const god::cursor& cursor);
```
**Returns**: [`bool`] - True if we are at the end of input, false otherwise.

---
#### `god::tokenize`
Process input into a list of [`god::token`](#godtoken)s.
| name      | type                              |
|-----------|-----------------------------------|
| **lines** | `const std::vector<std::string>&` |
```cpp
std::vector<god::token> god::tokenize(const std::vector<std::string>& lines);
```
**Return**: [`std::vector<god::token>`] - A list of [`god::token`](#godtoken)s.