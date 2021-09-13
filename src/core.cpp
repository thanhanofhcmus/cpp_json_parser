#include "json_parser/core.h"
#include "json_parser/detail/Scanner.h"
#include "json_parser/detail/Parser.h"

namespace json {

auto parse_string(std::string const& source) -> Json {
    return Parser(Scanner(source).scan()).parse();
}

} // namespace json