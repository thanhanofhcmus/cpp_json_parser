#include "core.h"
#include "Scanner.h"
#include "Parser.h"

namespace json {

auto parse_string(std::string const& source) -> Json {
    return Parser(Scanner(source).scan()).parse();
}

} // namespace json