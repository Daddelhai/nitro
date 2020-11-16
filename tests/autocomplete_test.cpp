#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <nitro/broken_options/autocomplete_builder.hpp>

TEST_CASE("Autocomplete option gives correct value")
{
    std::stringstream ss{};

    nitro::broken_options::parser parser("app_name", "about");
    auto opt1 = nitro::broken_options::autocompletion::option(
        parser.option("opt1", "some opt").short_name("o"));
    auto opt2 = nitro::broken_options::autocompletion::option(parser.option("opt2", "some opt"));
    auto opt3 = nitro::broken_options::autocompletion::option(
        parser.option("opt3", "some opt").short_name("u"));

    opt3.force_long_name = true;

    opt1.format(ss);
    opt2.format(ss);
    opt3.format(ss);

    REQUIRE(ss.str() == "-o --opt --opt2 --opt3");
}

TEST_CASE("Autocomplete creates correct complete command")
{
    nitro::broken_options::parser parser("app_name", "about");
    parser.option("opt1", "some opt").short_name("o");
    parser.option("opt2", "some opt");
    parser.option("opt3", "some opt").short_name("u");

    nitro::broken_options::autocompletion::builder builder(parser);

    REQUIRE(builder.get() == "complete - W \"-o -opt1 --opt2 -u --opt3\"");
}
