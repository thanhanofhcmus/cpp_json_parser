#pragma once

#include "Token.h"
#include "JsonValue.h"
#include <vector>

/*
json    = element
element = literal | object | array
literal = "null" | "true" | "false" | STRING | NUMBER
array   = "[" (element ",")* "]"
object  = "{" (STRING ":" element)* "}"
*/

namespace json {

class Parser {
public:

    Parser(std::vector<Token>);

    auto parse() -> Json;

private:

    struct ObjectElemParsingResult {
        Json::object_elem_t kv;
        Token const& key_token;
    };

private:

    // returning an additional key token for error handling
    [[nodiscard]] auto parse_object_elem() -> ObjectElemParsingResult;

    [[nodiscard]] auto parse_element() -> Json;
    [[nodiscard]] auto parse_literal() -> Json;
    [[nodiscard]] auto parse_array()   -> Json;
    [[nodiscard]] auto parse_object()  -> Json;

    void insert_object_elem(Json::object_t& object);

    [[nodiscard]] auto is_not_end() const noexcept -> bool;
    [[nodiscard]] auto match(TokenType type) -> bool;
    [[nodiscard]] auto match(std::initializer_list<TokenType> list) -> bool;
    [[nodiscard]] auto check(TokenType type) const -> bool;
    [[nodiscard]] auto peek() const -> const Token&;
    [[nodiscard]] auto previous() const -> const Token&;
    auto advance() -> Token&;
    auto consume(TokenType type, std::string const& message) -> Token&;

private:

    std::vector<Token> source_;
    std::size_t current_;

};

} // namespace json