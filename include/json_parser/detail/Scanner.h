#pragma once

#include "Token.h"
#include <vector>
#include <string_view>

namespace json {

class Scanner {
public:

    Scanner(std::string const& source);

    auto scan() && -> std::vector<Token>;

private:

    void scan_token();

    void scan_number();
    void scan_string();
    void scan_identifier();
    [[nodiscard]] auto scan_escape_sequence() -> char;
    [[nodiscard]] auto scan_hex_digit() -> char;

    [[nodiscard]] auto is_on_digit(char c) const -> bool;
    [[nodiscard]] auto is_not_end() const -> bool;

    auto advance() -> char;
    [[nodiscard]] auto peek() const -> char;

    void update_start_position();
    void add_token(TokenType type, Token::literal_t literal = Token::NullLiteral{});

    void error(std::string const& message) const;

private:

    std::vector<Token> tokens_;
    std::string_view source_;
    std::size_t start_;
    std::size_t start_line_;
    std::size_t start_column_;
    std::size_t current_;
    std::size_t current_line_;
    std::size_t current_column_;
};

} // namespace json