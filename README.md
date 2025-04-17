# `ct::String`

[![CI](https://github.com/ChrisThrasher/ct_string/actions/workflows/ci.yml/badge.svg)](https://github.com/ChrisThrasher/ct_string/actions/workflows/ci.yml)

`ct::String` is a C++20 Unicode-aware string class which exists to prove that `std::filesystem::path` can be used as a general purpose Unicode string conversion utility.
It is tested to on all major platforms.

> **This is not a serious project.
> It is a proof of concept.
> I do not intend for you to use this for any application where robustness, correctness, or performance are important.**

## Features

* Construct from any C++ string type
  * Any character type: `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`
  * Any string container: Character arrays, `std::basic_string`, `std::basic_string_view`
* Convert to any string type: `std::string`, `std::wstring`, `std::u8string`, `std::u16string`, `std::u32string`
* Get Nth codepoint
* Iterate by codepoint
* Push/pop back codepoint
* Append string
* Insert string

## So what's this about `std::filesystem::path`?

`std::filesystem::path` is a class added in C++17's `<filesystem>` library that abstracts over the concept of a filepath on disk as the name would imply.
It's basically just a string with an interface for doing useful things with filepaths.
As a circumstance of its design, it can be constructed from any string type.
This means you can use any of the string encodings C++ offers: ASCII, UTF-8, UTF-16, and UTF-32.
It also has member functions which will output a string in any of those same encodings.
When you combine those consructors with those member functions, you can convert a string from one encoding to another!

"So what?" you may ask. Surely C++ already offers a standard way to, for example, convert a UTF-16 string to UTF-8.
Nope!
Such functionalities were deprecated in C++20 and frankly they were not very user-friendly to begin with.
You may try to use them but your implementation is prone to remove them.
However due to way `std::filesystem::path` is specified, it must continue to support this transcoding functionality at least until the ISO C++ committee sees this abomination of a library and removes it.

## Usage

Use `find_package(ct_string REQUIRED)` to find ct_string then link to the `CT::String` CMake target to get access to everything required to use the library.

```cmake
target_link_libraries(<target> PRIVATE CT::String)
```

Then include the `<CT/String.hpp>` header file

```cpp
#include <CT/String.hpp>
#include <iostream>

int main()
{
    // Create string from a variety of encodings
    ct::String string(u8R"(¯\_()"); // UTF-8
    string.push_back(u'ツ'); // UTF-16
    string.append(U")_/¯"); // UTF-32
    std::cout << string.to_string(); // "¯\_(ツ)_/¯"

    // Output to any encoding of your choice
    const std::string ascii = string.to_string();
    const std::wstring wide = string.to_wstring(); // UTF-16 on Windows, UTF-32 elsewhere
    const std::u8string u8string = string.to_u8string();
    const std::u16string u16string = string.to_u16string();
    const std::u32string u32string = string.to_u32string();
}
```

## Shortcomings

Achieving all this functionality with just `std::filesystem::path` means doing some ugly things under the hood.
I expect the performance of this class to be very poor compared to a proper Unicode string class.

Furthermore, error handling is quite limited.
Constructing a `ct::String` from invalid UTF-8 for example produces unspecified results on different platforms.
This class assumes that all inputs are valid to begin with.
