#include "json_parser/core.h"
#include "detail/Scanner.h"
#include "detail/Parser.h"

namespace json {

auto parse_string(std::string const& source) -> Json {
    return Parser(Scanner(source).scan()).parse();
}

} // namespace json