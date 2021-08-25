#include <iostream>
#include "Parser.h"
#include "Scanner.h"
#include "core.h"

auto main() -> int {
    [[maybe_unused]] auto s1 = R"(
[true, false, null, { "a number": 1.2, "a string" : "something", "a": 1, "a": 2 }, 12e-1, "another string" ])";
    [[maybe_unused]] auto s2 = R"({ "number_1" : 210, "number_2" : -210, "number_3" : 21.05, "number_4" : 1.0E+2 })";
    [[maybe_unused]] auto s3 = R"({ "a": 1, "b": true, "c": null, d: "string" })";
    [[maybe_unused]] auto s4 = R"([ "\u0042", true])";
    // std::cout << json::parse_string(R"(true false)").to_string() << '\n';
    std::cout << json::parse_string(s4).to_string() << '\n';
}