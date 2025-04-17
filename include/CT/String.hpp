#pragma once

#include <filesystem>
#include <ostream>
#include <string>
#include <string_view>

namespace ct {
class String {
public:
    String() = default;
    String(std::string_view string);
    String(std::wstring_view string);
    String(std::u8string_view string);
    String(std::u16string_view string);
    String(std::u32string_view string);

    class ConstIterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using value_type = char32_t;

        ConstIterator() = default;
        ConstIterator& operator++();
        [[nodiscard]] ConstIterator operator++(int);
        [[nodiscard]] char32_t operator*() const;
        [[nodiscard]] bool operator==(const ConstIterator& it) const = default;
        friend std::ostream& operator<<(std::ostream& stream, const ConstIterator& it);

    private:
        friend String;

        ConstIterator(const String* string, std::size_t index);

        const String* m_parent {};
        std::size_t m_index {};
    };

    [[nodiscard]] ConstIterator begin() const;
    [[nodiscard]] ConstIterator end() const;

    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] std::wstring to_wstring() const;
    [[nodiscard]] std::u8string to_u8string() const;
    [[nodiscard]] std::u16string to_u16string() const;
    [[nodiscard]] std::u32string to_u32string() const;

    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] std::size_t codepoint_count() const;
    [[nodiscard]] char32_t codepoint_at(std::size_t index) const;

    void clear();

    void push_back(char codepoint);
    void push_back(wchar_t codepoint);
    void push_back(char8_t codepoint);
    void push_back(char16_t codepoint);
    void push_back(char32_t codepoint);

    void pop_back_codepoint();

    void append(std::string_view string);
    void append(std::wstring_view string);
    void append(std::u8string_view string);
    void append(std::u16string_view string);
    void append(std::u32string_view string);

    void insert(std::size_t index, std::string_view string);
    void insert(std::size_t index, std::wstring_view string);
    void insert(std::size_t index, std::u8string_view string);
    void insert(std::size_t index, std::u16string_view string);
    void insert(std::size_t index, std::u32string_view string);

    [[nodiscard]] bool operator==(const String&) const = default;

private:
    std::filesystem::path m_string;
};

std::ostream& operator<<(std::ostream& stream, const String& string);
}
