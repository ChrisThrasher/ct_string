#include <CT/String.hpp>

#include <cassert>
#include <format>
#include <iomanip>
#include <source_location>

namespace {
void verify(const bool condition,
            const std::string_view message,
            const std::source_location& source_location = std::source_location::current())
{
    if (condition)
        return;

    throw std::runtime_error(std::format("Assertion failed at {}:{} in `{}`\n{}",
                                         source_location.file_name(),
                                         source_location.line(),
                                         source_location.function_name(),
                                         message));
}

template <typename Char>
void insert(ct::String& ct_string,
            const std::size_t index,
            const std::basic_string_view<Char> string,
            const std::source_location& source_location)
{
    auto u32string = ct_string.to_u32string();
    verify(index < u32string.size(), "Index out of bounds", source_location);
    ct_string = ct::String(u32string.insert(index, std::filesystem::path(string).u32string()));
}
}

namespace ct {
String::String(const std::string_view string)
    : m_string(string)
{
}

String::String(const std::wstring_view string)
    : m_string(string)
{
}

String::String(const std::u8string_view string)
    : m_string(string)
{
}

String::String(const std::u16string_view string)
    : m_string(string)
{
}

String::String(const std::u32string_view string)
    : m_string(string)
{
}

String::ConstIterator& String::ConstIterator::operator++()
{
    ++m_index;
    return *this;
}

String::ConstIterator String::ConstIterator::operator++(int)
{
    const auto copy = *this;
    ++m_index;
    return copy;
}

char32_t String::ConstIterator::operator*() const
{
    assert(m_parent);
    return m_parent->codepoint_at(m_index);
}

String::ConstIterator::ConstIterator(const String* string, const std::size_t index)
    : m_parent(string)
    , m_index(index)
{
    assert(m_parent);
}

String::ConstIterator String::begin() const { return { this, 0 }; }

String::ConstIterator String::end() const { return { this, codepoint_count() }; }

std::string String::to_string() const
try {
    return m_string.string();
} catch (const std::exception&) {
    const auto u8string = to_u8string();
    return { reinterpret_cast<const char*>(u8string.data()), u8string.size() };
}

std::wstring String::to_wstring() const
try {
    return m_string.wstring();
} catch (const std::exception&) {
    return {};
}

std::u8string String::to_u8string() const
try {
    return m_string.u8string();
} catch (const std::exception&) {
    return {};
}

std::u16string String::to_u16string() const
try {
    return m_string.u16string();
} catch (const std::exception&) {
    return {};
}

std::u32string String::to_u32string() const
try {
    return m_string.u32string();
} catch (const std::exception&) {
    return {};
}

bool String::is_empty() const { return m_string.empty(); }

std::size_t String::codepoint_count() const { return to_u32string().size(); }

char32_t String::codepoint_at(const std::size_t index) const
{
    const auto u32string = to_u32string();
    verify(index < u32string.size(), "Index out of bounds");
    return u32string[index];
}

void String::clear() { m_string.clear(); }

void String::push_back(const char codepoint) { m_string += codepoint; }

void String::push_back(const wchar_t codepoint) { m_string += codepoint; }

void String::push_back(const char8_t codepoint) { m_string += codepoint; }

void String::push_back(const char16_t codepoint) { m_string += codepoint; }

void String::push_back(const char32_t codepoint) { m_string += codepoint; }

void String::pop_back_codepoint()
{
    auto u32string = to_u32string();
    verify(!u32string.empty(), "Cannot pop from an empty string");
    u32string.pop_back();
    m_string = u32string;
}

void String::append(const std::string_view string) { m_string += string; }

void String::append(const std::wstring_view string) { m_string += string; }

void String::append(const std::u8string_view string) { m_string += string; }

void String::append(const std::u16string_view string) { m_string += string; }

void String::append(const std::u32string_view string) { m_string += string; }

void String::insert(const std::size_t index, const std::string_view string)
{
    ::insert(*this, index, string, std::source_location::current());
}

void String::insert(const std::size_t index, const std::wstring_view string)
{
    ::insert(*this, index, string, std::source_location::current());
}

void String::insert(const std::size_t index, const std::u8string_view string)
{
    ::insert(*this, index, string, std::source_location::current());
}

void String::insert(const std::size_t index, const std::u16string_view string)
{
    ::insert(*this, index, string, std::source_location::current());
}

void String::insert(const std::size_t index, const std::u32string_view string)
{
    ::insert(*this, index, string, std::source_location::current());
}

std::ostream& operator<<(std::ostream& stream, const String& string)
{
    return stream << std::quoted(string.to_string());
}

std::ostream& operator<<(std::ostream& stream, const String::ConstIterator& it)
{
    return stream << "Parent string: " << it.m_parent << ", Index: " << it.m_index;
}
}
