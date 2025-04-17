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
