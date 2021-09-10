#pragma once

#include <string>
#include <variant>

#include <iostream>

namespace json {

enum class TokenType : std::uint8_t {
    Null,           // "null"
    True,           // "true"
    False,          // "false"
    Number,
    String,

    Comma,          // ","
    Colon,          // ":"
    LeftBrace,      // "{"
    RightBrace,     // "}"
    LeftBracket,    // "["
    RightBracket,   // "]"

    Eof,
};

struct Token {
    struct NullLiteral { };
    using literal_t = std::variant<std::string, double, NullLiteral>;

    int line;
    int column;
    TokenType type;
    literal_t literal = NullLiteral{};

    Token() =  default;
    Token(int _line, int _column, TokenType _type, literal_t _literal);

    [[nodiscard]] auto to_string() const -> std::string;

};

} // namespace json