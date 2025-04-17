#include <CT/String.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <format>
#include <sstream>

namespace {
template <typename Char>
[[nodiscard]] std::string format_character(const Char character)
{
    return std::format("\\x{:X}", +character);
}
}

template <>
struct Catch::StringMaker<wchar_t> {
    static std::string convert(const wchar_t character) { return format_character(character); }
};

template <>
struct Catch::StringMaker<char8_t> {
    static std::string convert(const char8_t character) { return format_character(character); }
};

template <>
struct Catch::StringMaker<char16_t> {
    static std::string convert(const char16_t character) { return format_character(character); }
};

template <>
struct Catch::StringMaker<char32_t> {
    static std::string convert(const char32_t character) { return format_character(character); }
};

TEST_CASE("ct::String")
{
    using Catch::Matchers::ContainsSubstring;

    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_default_constructible_v<ct::String>);
        STATIC_CHECK(std::is_copy_constructible_v<ct::String>);
        STATIC_CHECK(std::is_copy_assignable_v<ct::String>);
        STATIC_CHECK(std::is_move_constructible_v<ct::String>);
        STATIC_CHECK(std::is_move_assignable_v<ct::String>);

        STATIC_CHECK(sizeof(ct::String) == sizeof(std::filesystem::path));
        STATIC_CHECK(alignof(ct::String) == alignof(std::filesystem::path));
    }

    SECTION("Construction")
    {
        SECTION("std::string_view constructor")
        {
            const ct::String string("asdf");
            CHECK(string.to_string() == "asdf");
            CHECK(string.to_wstring() == L"asdf");
            CHECK(string.to_u8string() == u8"asdf");
            CHECK(string.to_u16string() == u"asdf");
            CHECK(string.to_u32string() == U"asdf");
        }

        SECTION("std::wstring_view constructor")
        {
            const ct::String string(L"⥇⏚≆⥰⊘⁤");
            CHECK(string.to_string() == "⥇⏚≆⥰⊘⁤");
            CHECK(string.to_wstring() == L"⥇⏚≆⥰⊘⁤");
            CHECK(string.to_u8string() == u8"⥇⏚≆⥰⊘⁤");
            CHECK(string.to_u16string() == u"⥇⏚≆⥰⊘⁤");
            CHECK(string.to_u32string() == U"⥇⏚≆⥰⊘⁤");
        }

        SECTION("std::u8string_view constructor")
        {
            const ct::String string(u8"ⵒ⩦⓷⻠⭅⩋");
            CHECK(string.to_string() == "ⵒ⩦⓷⻠⭅⩋");
            CHECK(string.to_wstring() == L"ⵒ⩦⓷⻠⭅⩋");
            CHECK(string.to_u8string() == u8"ⵒ⩦⓷⻠⭅⩋");
            CHECK(string.to_u16string() == u"ⵒ⩦⓷⻠⭅⩋");
            CHECK(string.to_u32string() == U"ⵒ⩦⓷⻠⭅⩋");
        }

        SECTION("std::u16string_view constructor")
        {
            const ct::String string(u"⻍⳿✈⤆┹⥪");
            CHECK(string.to_string() == "⻍⳿✈⤆┹⥪");
            CHECK(string.to_wstring() == L"⻍⳿✈⤆┹⥪");
            CHECK(string.to_u8string() == u8"⻍⳿✈⤆┹⥪");
            CHECK(string.to_u16string() == u"⻍⳿✈⤆┹⥪");
            CHECK(string.to_u32string() == U"⻍⳿✈⤆┹⥪");
        }

        SECTION("std::u32string_view constructor")
        {
            const ct::String string(U"⨸⟇↸≙⍨⼦");
            CHECK(string.to_string() == "⨸⟇↸≙⍨⼦");
            CHECK(string.to_wstring() == L"⨸⟇↸≙⍨⼦");
            CHECK(string.to_u8string() == u8"⨸⟇↸≙⍨⼦");
            CHECK(string.to_u16string() == u"⨸⟇↸≙⍨⼦");
            CHECK(string.to_u32string() == U"⨸⟇↸≙⍨⼦");
        }
    }

    SECTION("ConstIterator")
    {
        STATIC_CHECK(std::forward_iterator<ct::String::ConstIterator>);

        SECTION("Empty string")
        {
            const ct::String string;
            CHECK(string.begin() == string.end());
        }

        SECTION("Non-empty string")
        {
            const ct::String string(u8"έӭᔼő?");
            CHECK(string.begin() != string.end());
            auto begin = string.begin();
            CHECK(*begin++ == U'έ');
            CHECK(*begin++ == U'ӭ');
            CHECK(*begin++ == U'ᔼ');
            CHECK(*begin++ == U'ő');
            CHECK(*begin++ == U'?');
            CHECK(begin == string.end());
            CHECK_THROWS_WITH(
                *begin, ContainsSubstring("ct::String::codepoint_at") && ContainsSubstring("Index out of bounds"));
        }

        SECTION("Ranged for loop")
        {
            std::u32string u32string;
            const ct::String string(uR"(¯\_(ツ)_/¯)");
            for (const auto character : string)
                u32string.push_back(character);
            CHECK(u32string == UR"(¯\_(ツ)_/¯)");
        }
    }

    SECTION("is_empty")
    {
        CHECK(ct::String().is_empty());
        CHECK(ct::String(GENERATE("", L"", u8"", u"", U"")).is_empty());
        CHECK(!ct::String(" ").is_empty());
        CHECK(!ct::String("⥝⠂⁉ⵆ⟚ⅺ⌞⡘⁐⧤⿅⒘⻮ⱦ⓲⿁♜ⅰⶢ⿋⋱⺂⡯↠✷℩⦱⩻").is_empty());
    }

    SECTION("codepoint_count")
    {
        CHECK(ct::String().codepoint_count() == 0);
        CHECK(ct::String("").codepoint_count() == 0);
        CHECK(ct::String(" ").codepoint_count() == 1);
        CHECK(ct::String("asdf").codepoint_count() == 4);
        CHECK(ct::String(u8"🙂").codepoint_count() == 1);
        CHECK(ct::String(u8"👨🏻‍💻").codepoint_count() == 4);
    }

    SECTION("codepoint_at")
    {
        const ct::String string(u8"aℬ©déf⌘🙂👨🏻‍💻");
        CHECK(string.codepoint_at(0) == U'a');
        CHECK(string.codepoint_at(1) == U'ℬ');
        CHECK(string.codepoint_at(2) == U'©');
        CHECK(string.codepoint_at(3) == U'd');
        CHECK(string.codepoint_at(4) == U'é');
        CHECK(string.codepoint_at(5) == U'f');
        CHECK(string.codepoint_at(6) == U'⌘');
        CHECK(string.codepoint_at(7) == U'🙂');
        CHECK(string.codepoint_at(8) == U'👨');
        CHECK(string.codepoint_at(9) == U'\U0001F3FB');
        CHECK(string.codepoint_at(10) == U'\u200D');
        CHECK(string.codepoint_at(11) == U'💻');

        CHECK_THROWS_WITH(string.codepoint_at(12),
                          ContainsSubstring("ct::String::codepoint_at") && ContainsSubstring("Index out of bounds"));
    }

    SECTION("clear")
    {
        ct::String string(GENERATE("asdf", L"asdf", u8"asdf", u"asdf", U"asdf"));
        string.clear();
        CHECK(string.to_string().empty());
        CHECK(string.is_empty());
        CHECK(string.codepoint_count() == 0);
    }

    SECTION("push_back")
    {
        ct::String string;
        string.push_back('a');
        CHECK(string.to_u8string() == u8"a");
        string.push_back(L'ß');
        CHECK(string.to_u8string() == u8"aß");
        string.push_back(u8'c');
        CHECK(string.to_u8string() == u8"aßc");
        string.push_back(u'Ď');
        CHECK(string.to_u8string() == u8"aßcĎ");
        string.push_back(U'😎');
        CHECK(string.to_u8string() == u8"aßcĎ😎");
    }

    SECTION("pop_back_codepoint")
    {
        ct::String string(u8"╵ⱱ⨁⺄⌦");
        string.pop_back_codepoint();
        CHECK(string.to_u8string() == u8"╵ⱱ⨁⺄");
        string.pop_back_codepoint();
        CHECK(string.to_u8string() == u8"╵ⱱ⨁");
        string.pop_back_codepoint();
        CHECK(string.to_u8string() == u8"╵ⱱ");
        string.pop_back_codepoint();
        CHECK(string.to_u8string() == u8"╵");
        string.pop_back_codepoint();
        CHECK(string.is_empty());

        CHECK_THROWS_WITH(string.pop_back_codepoint(),
                          ContainsSubstring("ct::String::pop_back_codepoint")
                              && ContainsSubstring("Cannot pop from an empty string"));
    }

    SECTION("append")
    {
        ct::String string;
        string.append("ab");
        CHECK(string.to_u8string() == u8"ab");
        string.append(L"çĎ");
        CHECK(string.to_u8string() == u8"abçĎ");
        string.append(u8"éf");
        CHECK(string.to_u8string() == u8"abçĎéf");
        string.append(u"gḥ");
        CHECK(string.to_u8string() == u8"abçĎéfgḥ");
        string.append(U"íj");
        CHECK(string.to_u8string() == u8"abçĎéfgḥíj");
    }

    SECTION("insert")
    {
        ct::String string("abef");
        string.insert(2, "cd");
        CHECK(string.to_u8string() == u8"abcdef");
        string.insert(0, L"Ⓦ⻀ⶌ");
        CHECK(string.to_u8string() == u8"Ⓦ⻀ⶌabcdef");
        string.insert(8, u8"⠁∅⡬");
        CHECK(string.to_u8string() == u8"Ⓦ⻀ⶌabcde⠁∅⡬f");
        string.insert(4, u"ⵦ℆ⷉ");
        CHECK(string.to_u8string() == u8"Ⓦ⻀ⶌaⵦ℆ⷉbcde⠁∅⡬f");
        string.insert(10, U"⾿ⳗ↴");
        CHECK(string.to_u8string() == u8"Ⓦ⻀ⶌaⵦ℆ⷉbcd⾿ⳗ↴e⠁∅⡬f");

        CHECK_THROWS_WITH(string.insert(1'000, GENERATE("", L"", u8"", u"", U"")),
                          ContainsSubstring("ct::String::insert") && ContainsSubstring("Index out of bounds"));
    }

    SECTION("operator==")
    {
        CHECK(ct::String() == ct::String());
        CHECK(ct::String("") == ct::String(""));
        CHECK(ct::String(" ") == ct::String(u8" "));
        CHECK(ct::String(u"!?") == ct::String(U"!?"));
    }

    SECTION("operator!=")
    {
        CHECK(ct::String() != ct::String(" "));
        CHECK(ct::String(" ") != ct::String(""));
        CHECK(ct::String("a") != ct::String(u8"b"));
        CHECK(ct::String(L"test") != ct::String(L"tset"));
    }

    SECTION("operator<<")
    {
        std::ostringstream stream;
        stream << ct::String("Hello 👋");
        CHECK(stream.str() == R"("Hello 👋")");
    }
}
