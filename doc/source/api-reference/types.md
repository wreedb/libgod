Types
=====
This page documents API specific type declarations.

Unions
------

#### `god::type::value`
A union of the different **God** types in their C++ forms.
| type             | description                                                                          |
|------------------|--------------------------------------------------------------------------------------|
| `std::string`    | Used for both normal and multi-line strings                                          |
| `std::int64_t`   | The basic number type for integers that would not be better represented by `double`s |
| `double`         | Used for numbers that fall outside the scope of `std::int64_t`                       |
| `bool`           | Represents the `true` and `false` God types.                                         |
| `std::nullptr_t` | Represents the `null` God type.                                                      |
| `map`            | see [`god::type::map`](#godtypemap)                                                  |
| `list`           | see [`god::type::list`](#godtypelist)                                                |
```cpp
// Within namespace `god::type`
using value = std::variant<
    std::string,
    std::int64_t,
    double,
    bool,
    std::nullptr_t,
    map,
    list
>;
```

---
Structs
-------

#### `god::type::map`
An unordered key-value mapping of identifiers to values
| name    | type                                                |
|---------|-----------------------------------------------------|
| members | `std::unordered_map<std::string, god::type::value>` |
```cpp
struct god::type::map {
    std::unordered_map<std::string, god::type::value> members;
};
```

#### `god::type::list`
A list of **God** values.
| name     | type                            |
|----------|---------------------------------|
| elements | `std::vector<god::type::value>` |
```cpp
struct god::type::list {
    std::vector<god::type::value> elements;
};
```
~~~admonish note
Both `god::type::map` and `god::type::list` can contain elements/members of their own type (e.g.: lists/maps inside lists/maps).
~~~